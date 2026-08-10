// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Auth/AACraftItem.h"

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Asset/TradeAsset.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetRuleDefinition.h"
#include "Core/Type/Runtime/CraftInstance.h"
#include "Definition/Runtime/TradeKey.h"
#include "Interface/AssetInstanceCollection.h"
#include "Interface/AssetInstanceCollectionProvider.h"
#include "Library/AssetInstanceUtil.h"
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollection_Trade.h"
#include "Manager/RAssetManager.inl"
#include "Core/CraftSettings.h"
#include "System/CraftStorageManager.h"
#include "System/CraftSubsystem.h"
#include "Data/CraftFragment.h"


void UAACraftItem::OnStarted()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());
	Step_LoadAsset();
}

void UAACraftItem::OnCompleted(bool bSuccess)
{
	AssetManager->CancelFetch(ActionId);
}

void UAACraftItem::OnCleanup()
{
	AssetManager = nullptr;
	TradeAsset = nullptr;
	TargetAsset = nullptr;
	MaterialTransaction = nullptr;

	CraftAssetId = FPrimaryAssetId();
	TargetAssetId = FPrimaryAssetId();

	TargetQuantity = 0;
}

void UAACraftItem::Step_LoadAsset()
{
	if (!IsValid(AssetManager))
	{
		Fail(TEXT("AssetManager is invalid"));
		return;
	}

	TArray<FPrimaryAssetId> Assets;
	Assets.Add(CraftAssetId);
	Assets.Add(TargetAssetId);

	TFuture<FLatentLoadedAssets<UCoreDataAsset>> Future = AssetManager->FetchPrimaryAssets<UCoreDataAsset>(ActionId, Assets);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UAACraftItem> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAssets<UCoreDataAsset>& Result)
		{
			UAACraftItem* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			const TArray<UCoreDataAsset*>& Assets = Result.Get();

			This->TradeAsset = Cast<UTradeAsset>(Assets[0]);
			This->TargetAsset = Assets[1];

			This->Step_CheckTargetAsset();
		}
	);
}

void UAACraftItem::Step_CheckTargetAsset()
{
	if (!IsValid(TradeAsset))
	{
		Fail(TEXT("TradeAsset is invalid"));
		return;
	}

	const UAssetGroup* TradeGroup = TradeAsset->TradeGroup;
	if (!IsValid(TradeGroup))
	{
		Fail(TEXT("Shop items is invalid"));
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

	Step_CheckMaterialAsset();
}

void UAACraftItem::Step_CheckMaterialAsset()
{
	const UCraftFragment* CraftFragment = TargetAsset->FindFragmentByClass<UCraftFragment>();
	if (!IsValid(CraftFragment))
	{
		Fail(TEXT("Item asset does not implement ICraftProvider"));
		return;
	}

	CraftingTime = CraftFragment->GetCraftingTime();

	FInstancedStruct MaterialContext = FInstancedStruct::Make(FAssetRuleContext(TradeCollectionId));
	const UAssetCollection* MaterialCollection = CraftFragment->GetCraftingMaterial(MaterialContext);
	if (!IsValid(MaterialCollection))
	{
		Fail(TEXT("Crafting material is invalid"));
		return;
	}

	TMap<FPrimaryAssetId, int> MaterialAssetList;
	MaterialCollection->GetAssetList(MaterialAssetList);

	FPrimaryAssetType MaterialAssetType = MaterialCollection->GetCollectionType();

	Step_CheckMaterialTransaction(MoveTemp(MaterialAssetList), MaterialAssetType);
}

void UAACraftItem::Step_CheckMaterialTransaction(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
{
	IAssetInstanceCollectionProvider* MaterialInterchange = FAssetInstanceUtil::GetInstanceCollectionProvider(GetWorld(), MaterialAssetType);
	if (!MaterialInterchange)
	{
		Fail(TEXT("Failed to get transaction interface"));
		return;
	}

	FName MaterialSourceId = MaterialInterchange->GetPrimaryCollectionId();
	MaterialTransaction = MaterialInterchange->GetInstanceCollection(MaterialSourceId);
	if (!MaterialTransaction)
	{
		Fail(TEXT("Failed to get transaction source"));
		return;
	}

	if (!MaterialTransaction->ContainInstances(MaterialAssetList, 1))
	{
		Fail(TEXT("Material not enough"));
		return;
	}

	Step_CheckCraftQuota(MoveTemp(MaterialAssetList), MaterialAssetType);
}

void UAACraftItem::Step_CheckCraftQuota(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
{
	int CraftingSeconds = CraftingTime.GetSeconds();
	if (TargetQuota <= 0 || CraftingSeconds <= 0)
	{
		Step_PerformTransaction(MoveTemp(MaterialAssetList), MaterialAssetType);
		return;
	}

	UCraftSubsystem* CraftSubsystem = UCraftSubsystem::Get(GetWorld());
	if (!IsValid(CraftSubsystem))
	{
		Fail(TEXT("Failed to get CraftSubsystem"));
		return;
	}

	UCraftStorageManager* StorageManager = CraftSubsystem->GetStorageManager();
	if (!IsValid(StorageManager))
	{
		Fail(TEXT("Failed to get CraftStorage"));
		return;
	}

	FTradeKey TradeKey(CraftAssetId, TradeCollectionId, TargetAssetId);
	const FCraftInstance* CraftData = StorageManager->GetItem(TradeKey);
	if (!CraftData)
	{
		Fail(TEXT("Failed to get craft data"));
		return;
	}

	if (CraftData->PendingQuantity >= TargetQuota)
	{
		Fail(TEXT("Item quota exceeded"));
		return;
	}

	if (!StorageManager->AddItem(TradeKey, CraftingTime))
	{
		Fail(TEXT("Failed to add craft item"));
		return;
	}

	Success();
}

void UAACraftItem::Step_PerformTransaction(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
{
	IAssetInstanceCollectionProvider* TargetInterchange = FAssetInstanceUtil::GetInstanceCollectionProvider(GetWorld(), TargetAssetId);
	if (!TargetInterchange)
	{
		Fail(TEXT("Failed to get target transaction interface"));
		return;
	}

	FName TargetSourceId = TargetInterchange->GetPrimaryCollectionId();
	IAssetInstanceCollection* TargetTransaction = TargetInterchange->GetInstanceCollection(TargetSourceId);
	if (!TargetTransaction || !MaterialTransaction)
	{
		Fail(TEXT("Failed to get transaction source"));
		return;
	}

	if (!MaterialTransaction->RemoveInstances(MaterialAssetList, 1))
	{
		Fail(TEXT("Failed to remove item"));
		return;
	}

	if (!TargetTransaction->AddInstance(TargetAssetId, TargetQuantity))
	{
		Fail(TEXT("Failed to add item"));
		return;
	}

	Success();
}
