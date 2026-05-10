// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/Task_PurchaseItem.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Asset/ShopAsset.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetRuleDefinition.h"
#include "Definition/Runtime/ShopData.h"
#include "Definition/Runtime/TradeKey.h"
#include "Interface/AssetInstanceCollection.h"
#include "Interface/AssetInstanceCollectionProvider.h"
#include "Interface/IShopProvider.h"
#include "Library/AssetInstanceUtil.h"
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollection_Trade.h"
#include "Manager/RAssetManager.inl"
#include "Settings/ShopSettings.h"
#include "Storage/ShopStorageManager.h"
#include "Subsystem/ShopSubsystem.h"


void UTask_PurchaseItem::OnStarted()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());
	Step_LoadAsset();
}

void UTask_PurchaseItem::OnCompleted(bool bSuccess)
{
	AssetManager->CancelFetch(ActionId);
}

void UTask_PurchaseItem::OnCleanup()
{
	AssetManager = nullptr;
	ShopAsset = nullptr;
	TargetAsset = nullptr;
	MaterialInstance = nullptr;

	ShopAssetId = FPrimaryAssetId();
	TargetAssetId = FPrimaryAssetId();

	TradeCollectionId.Invalidate();

	TargetQuantity = 0;
}

void UTask_PurchaseItem::Step_LoadAsset()
{
	if (!IsValid(AssetManager))
	{
		Fail(TEXT("AssetManager is invalid"));
		return;
	}
	
	TArray<FPrimaryAssetId> Assets;
	Assets.Add(ShopAssetId);
	Assets.Add(TargetAssetId);

	TFuture<FLatentLoadedAssets<UCoreDataAsset>> Future = AssetManager->FetchPrimaryAssets<UCoreDataAsset>(ActionId, Assets);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UTask_PurchaseItem> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAssets<UCoreDataAsset>& Result)
		{
			UTask_PurchaseItem* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			const TArray<UCoreDataAsset*>& Assets = Result.Get();

			This->ShopAsset = Cast<UShopAsset>(Assets[0]);
			This->TargetAsset = Assets[1];

			This->Step_CheckTarget();
		}
	);
}

void UTask_PurchaseItem::Step_CheckTarget()
{
	if (!IsValid(ShopAsset))
	{
		Fail(TEXT("Shop asset is invalid"));
		return;
	}

	const UAssetGroup* TradeGroup = ShopAsset->TradeGroup;
	if (!IsValid(TradeGroup))
	{
		Fail(TEXT("Trade group is invalid"));
		return;
	}

	FInstancedStruct TradeContext = FInstancedStruct::Make(FAssetRuleContext(TradeCollectionId));
	const UAssetCollection_Trade* TradeCollection = TradeGroup->GetCollectionRule<UAssetCollection_Trade>(TradeContext);
	if (!IsValid(TradeCollection))
	{
		Fail(TEXT("Item collection is invalid"));
		return;
	}

	FAssetDetail_Trade TargetDetail;
	if (!TradeCollection->GetAssetDetail(TargetAssetId, TargetDetail))
	{
		Fail(TEXT("Item asset not found"));
		return;
	}

	TargetQuantity = TargetDetail.Quantity;
	TargetQuota = TargetDetail.Quota;

	Step_CheckMaterial();
}

void UTask_PurchaseItem::Step_CheckMaterial()
{
	const IShopProvider* ShopProvider = Cast<IShopProvider>(TargetAsset);
	if (!ShopProvider)
	{
		Fail(TEXT("Item asset does not implement IShopProvider"));
		return;
	}

	FInstancedStruct MaterialContext = FInstancedStruct::Make(FAssetRuleContext(TradeCollectionId));
	const UAssetCollection* MaterialCollection = ShopProvider->GetPurchaseCost(MaterialContext);
	if (!IsValid(MaterialCollection))
	{
		Fail(TEXT("Purchase material is invalid"));
		return;
	}

	TMap<FPrimaryAssetId, int> MaterialAssetList;
	MaterialCollection->GetAssetList(MaterialAssetList);

	FPrimaryAssetType MaterialAssetType = MaterialCollection->GetCollectionType();

	Step_CheckMaterialTransaction(MoveTemp(MaterialAssetList), MaterialAssetType);
}

void UTask_PurchaseItem::Step_CheckMaterialTransaction(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
{
	IAssetInstanceCollectionProvider* MaterialInterchange = FAssetInstanceUtil::GetInstanceCollectionProvider(GetWorld(), MaterialAssetType);
	if (!MaterialInterchange)
	{
		Fail(TEXT("Failed to get transaction interface"));
		return;
	}

	FName MaterialSourceId = MaterialInterchange->GetPrimaryCollectionId();
	MaterialInstance = MaterialInterchange->GetInstanceCollection(MaterialSourceId);
	if (!MaterialInstance)
	{
		Fail(TEXT("Failed to get transaction source"));
		return;
	}

	if (!MaterialInstance->ContainInstances(MaterialAssetList, 1))
	{
		Fail(TEXT("Material not enough"));
		return;
	}

	Step_CheckShopQuota(MoveTemp(MaterialAssetList), MaterialAssetType);
}

void UTask_PurchaseItem::Step_CheckShopQuota(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
{
	if (TargetQuota <= 0)
	{
		Step_PerformTransaction(MoveTemp(MaterialAssetList), MaterialAssetType);
		return;
	}

	UShopSubsystem* ShopSubsystem = UShopSubsystem::Get(GetWorld());
	if (!IsValid(ShopSubsystem))
	{
		Fail(TEXT("Failed to get ShopSubsystem"));
		return;
	}

	UShopStorageManager* StorageManager = ShopSubsystem->GetStorageManager();
	if (!IsValid(StorageManager))
	{
		Fail(TEXT("Failed to get ShopStorage"));
		return;
	}

	FTradeKey TradeKey(ShopAssetId, TradeCollectionId, TargetAssetId);
	const FShopData* ShopData = StorageManager->GetItem(TradeKey);
	if (!ShopData)
	{
		Fail(TEXT("Failed to get ShopData"));
		return;
	}

	if (ShopData->PurchaseCount >= TargetQuota)
	{
		Fail(TEXT("Item quota exceeded"));
		return;
	}

	if (!StorageManager->AddItem(TradeKey))
	{
		Fail(TEXT("Failed to add item"));
		return;
	}

	Step_PerformTransaction(MoveTemp(MaterialAssetList), MaterialAssetType);
}

void UTask_PurchaseItem::Step_PerformTransaction(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
{
	IAssetInstanceCollectionProvider* TargetInterchange = FAssetInstanceUtil::GetInstanceCollectionProvider(GetWorld(), TargetAssetId);
	if (!TargetInterchange)
	{
		Fail(TEXT("Failed to get transaction interface"));
		return;
	}

	FName TargetSourceId = TargetInterchange->GetPrimaryCollectionId();
	IAssetInstanceCollection* TargetInstance = TargetInterchange->GetInstanceCollection(TargetSourceId);
	if (!TargetInstance || !MaterialInstance)
	{
		Fail(TEXT("Failed to get transaction source"));
		return;
	}

	if (!MaterialInstance->RemoveInstances(MaterialAssetList, 1))
	{
		Fail(TEXT("Failed to remove item"));
		return;
	}
	
	if (!TargetInstance->AddInstance(TargetAssetId, TargetQuantity))
	{
		Fail(TEXT("Failed to add item"));
		return;
	}

	Success();
}

