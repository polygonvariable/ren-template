// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/Task_CraftItem.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Asset/TradeAsset.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetRuleDefinition.h"
#include "Definition/Runtime/CraftData.h"
#include "Definition/Runtime/TradeKey.h"
#include "Interface/AssetInstanceCollection.h"
#include "Interface/AssetInstanceCollectionProvider.h"
#include "Interface/ICraftProvider.h"
#include "Library/AssetInstanceUtil.h"
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollection_Trade.h"
#include "Manager/RAssetManager.inl"
#include "Settings/CraftSettings.h"
#include "Storage/CraftStorage.h"
#include "Subsystem/CraftSubsystem.h"




void UTask_CraftItem::OnStarted()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());
	Step_LoadAsset();
}

void UTask_CraftItem::OnStopped()
{
	AssetManager->CancelFetch(TaskId);
}

void UTask_CraftItem::OnCleanup()
{
	AssetManager = nullptr;
	TradeAsset = nullptr;
	TargetAsset = nullptr;
	MaterialTransaction = nullptr;

	CraftAssetId = FPrimaryAssetId();
	TargetAssetId = FPrimaryAssetId();

	TargetQuantity = 0;
}

void UTask_CraftItem::Step_LoadAsset()
{
	if (!IsValid(AssetManager))
	{
		Fail(TEXT("AssetManager is invalid"));
		return;
	}

	TArray<FPrimaryAssetId> Assets;
	Assets.Add(CraftAssetId);
	Assets.Add(TargetAssetId);

	TFuture<FLatentLoadedAssets<UCoreDataAsset>> Future = AssetManager->FetchPrimaryAssets<UCoreDataAsset>(TaskId, Assets);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UTask_CraftItem> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAssets<UCoreDataAsset>& Result)
		{
			UTask_CraftItem* This = WeakThis.Get();
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

void UTask_CraftItem::Step_CheckTargetAsset()
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

void UTask_CraftItem::Step_CheckMaterialAsset()
{
	const ICraftProvider* CraftProvider = Cast<ICraftProvider>(TargetAsset);
	if (!CraftProvider)
	{
		Fail(TEXT("Item asset does not implement ICraftProvider"));
		return;
	}

	CraftingTime = CraftProvider->GetCraftingTime();

	FInstancedStruct MaterialContext = FInstancedStruct::Make(FAssetRuleContext(TradeCollectionId));
	const UAssetCollection* MaterialCollection = CraftProvider->GetCraftingMaterial(MaterialContext);
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

void UTask_CraftItem::Step_CheckMaterialTransaction(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
{
	IAssetInstanceCollectionProvider* MaterialInterchange = FAssetInstanceUtil::GetInstanceCollectionProvider(GetWorld(), MaterialAssetType);
	if (!MaterialInterchange)
	{
		Fail(TEXT("Failed to get transaction interface"));
		return;
	}

	FName MaterialSourceId = MaterialInterchange->GetDefaultCollectionId();
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

void UTask_CraftItem::Step_CheckCraftQuota(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
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

	FName CraftSourceId = UCraftSettings::Get()->StorageId;
	UCraftStorage* CraftStorage = CraftSubsystem->GetCraft(CraftSourceId);
	if (!IsValid(CraftStorage))
	{
		Fail(TEXT("Failed to get CraftStorage"));
		return;
	}

	FTradeKey TradeKey(CraftAssetId, TradeCollectionId, TargetAssetId);
	const FCraftData* CraftData = CraftStorage->GetItem(TradeKey);
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

	if (!CraftStorage->AddItem(TradeKey, CraftingTime))
	{
		Fail(TEXT("Failed to add craft item"));
		return;
	}

	Success();
}

void UTask_CraftItem::Step_PerformTransaction(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType)
{
	IAssetInstanceCollectionProvider* TargetInterchange = FAssetInstanceUtil::GetInstanceCollectionProvider(GetWorld(), TargetAssetId);
	if (!TargetInterchange)
	{
		Fail(TEXT("Failed to get target transaction interface"));
		return;
	}

	FName TargetSourceId = TargetInterchange->GetDefaultCollectionId();
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
