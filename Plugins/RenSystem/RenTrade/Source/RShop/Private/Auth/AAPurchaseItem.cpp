// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Auth/AAPurchaseItem.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Data/CoreDataAsset.h"
#include "Core/Type/Runtime/ShopInstance.h"
#include "Core/Type/Runtime/TradeKey.h"
#include "Data/ShopAsset.h"
#include "Data/ShopFragment.h"
#include "Data/TradeAssetCollection.h"
#include "Core/Type/AssetDetail.h"
#include "Core/Type/AssetRuleDefinition.h"
#include "Core/Interface/AssetInstanceCollection.h"
#include "Core/Interface/AssetInstanceCollectionProvider.h"
#include "Core/AssetInstanceUtil.h"
#include "Core/AssetManagerUtil.h"
#include "Data/AssetCollection.h"
#include "Data/AssetGroup.h"
#include "System/ShopStorageManager.h"
#include "System/ShopSubsystem.h"


void UAAPurchaseItem::OnStarted()
{
	AssetManager = UAssetManager::GetIfInitialized();
	Step_LoadAsset();
}

void UAAPurchaseItem::OnCompleted(bool bSuccess)
{
	FAssetManagerUtil::CancelHandle(_AssetHandle);
}

void UAAPurchaseItem::OnCleanup()
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

void UAAPurchaseItem::Step_LoadAsset()
{
	if (!IsValid(AssetManager))
	{
		Fail(TEXT("AssetManager is invalid"));
		return;
	}
	
	TArray<FPrimaryAssetId> Assets;
	Assets.Add(ShopAssetId);
	Assets.Add(TargetAssetId);

	FAssetManagerUtil::CancelHandle(_AssetHandle);

	_AssetHandle = AssetManager->LoadPrimaryAssets(Assets, TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UAAPurchaseItem::Step_HandleOnAssetsLoaded));
}

void UAAPurchaseItem::Step_HandleOnAssetsLoaded()
{
	FAssetManagerUtil::ReleaseHandle(_AssetHandle);

	ShopAsset = AssetManager->GetPrimaryAssetObject<UShopAsset>(ShopAssetId);
	TargetAsset = AssetManager->GetPrimaryAssetObject<UShopAsset>(TargetAssetId);

	Step_CheckTarget();
}

void UAAPurchaseItem::Step_CheckTarget()
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
	const UTradeAssetCollection* TradeCollection = TradeGroup->GetCollectionRule<UTradeAssetCollection>(TradeContext);
	if (!IsValid(TradeCollection))
	{
		Fail(TEXT("Item collection is invalid"));
		return;
	}

	FTradeAssetDetail TargetDetail;
	if (!TradeCollection->GetAssetDetail(TargetAssetId, TargetDetail))
	{
		Fail(TEXT("Item asset not found"));
		return;
	}

	TargetQuantity = TargetDetail.Quantity;
	TargetQuota = TargetDetail.Quota;

	Step_CheckMaterial();
}

void UAAPurchaseItem::Step_CheckMaterial()
{
	const UShopFragment* ShopFragment = TargetAsset->FindFragmentByClass<UShopFragment>();
	if (!IsValid(ShopFragment))
	{
		Fail(TEXT("Item asset does not implement IShopProvider"));
		return;
	}

	FInstancedStruct MaterialContext = FInstancedStruct::Make(FAssetRuleContext(TradeCollectionId));
	const UAssetCollection* MaterialCollection = ShopFragment->GetPurchaseCost(MaterialContext);
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

void UAAPurchaseItem::Step_CheckMaterialTransaction(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
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

void UAAPurchaseItem::Step_CheckShopQuota(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
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
	const FShopInstance* ShopInstance = StorageManager->GetItem(TradeKey);
	if (!ShopInstance)
	{
		Fail(TEXT("Failed to get ShopInstance"));
		return;
	}

	if (ShopInstance->PurchaseCount >= TargetQuota)
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

void UAAPurchaseItem::Step_PerformTransaction(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
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

