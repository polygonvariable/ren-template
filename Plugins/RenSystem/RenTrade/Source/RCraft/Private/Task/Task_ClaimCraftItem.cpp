// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/Task_ClaimCraftItem.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Asset/TradeAsset.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetRuleDefinition.h"
#include "Definition/Runtime/TradeKey.h"
#include "Interface/AssetInstanceCollection.h"
#include "Interface/AssetInstanceCollectionProvider.h"
#include "Library/AssetInstanceUtil.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollection_Trade.h"
#include "Manager/RAssetManager.inl"
#include "Settings/CraftSettings.h"
#include "Storage/CraftStorage.h"
#include "Subsystem/CraftSubsystem.h"




void UTask_ClaimCraftItem::OnStarted()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());
	Step_LoadAsset();
}

void UTask_ClaimCraftItem::OnStopped()
{
	AssetManager->CancelFetch(TaskId);
}

void UTask_ClaimCraftItem::OnCleanup()
{
	AssetManager = nullptr;
	TradeAsset = nullptr;

	CraftAssetId = FPrimaryAssetId();
	TargetAssetId = FPrimaryAssetId();

	TargetQuantity = 0;
	ClaimQuantity = 0;
}

void UTask_ClaimCraftItem::Step_LoadAsset()
{
	if (!IsValid(AssetManager))
	{
		Fail(TEXT("AssetManager is invalid"));
		return;
	}

	TFuture<FLatentLoadedAsset<UCoreDataAsset>> Future = AssetManager->FetchPrimaryAsset<UCoreDataAsset>(TaskId, CraftAssetId);
	TWeakObjectPtr<UTask_ClaimCraftItem> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAsset<UCoreDataAsset>& Result)
		{
			UTask_ClaimCraftItem* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			This->TradeAsset = Cast<UTradeAsset>(Result.Get());
			This->Step_CheckTarget();
		}
	);
}

void UTask_ClaimCraftItem::Step_CheckTarget()
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

	Step_CheckClaimable();
}

void UTask_ClaimCraftItem::Step_CheckClaimable()
{
	UWorld* World = GetWorld();
	UGameInstance* GameInstance = World->GetGameInstance();

	UCraftSubsystem* CraftSubsystem = UCraftSubsystem::Get(GameInstance);
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

	ClaimQuantity = CraftStorage->ClaimCraftedItems(FTradeKey(CraftAssetId, TradeCollectionId, TargetAssetId));
	if (ClaimQuantity <= 0)
	{
		Fail(TEXT("No item to claim"));
		return;
	}

	Step_PerformTransaction();
}

void UTask_ClaimCraftItem::Step_PerformTransaction()
{
	UWorld* World = GetWorld();
	UGameInstance* GameInstance = World->GetGameInstance();

	IAssetInstanceCollectionProvider* TargetInterchange = FAssetInstanceUtil::GetInstanceCollectionProvider(GameInstance, TargetAssetId);
	if (!TargetInterchange)
	{
		Fail(TEXT("Failed to get transaction interface"));
		return;
	}

	FName TargetSourceId = TargetInterchange->GetPrimaryCollectionId();
	IAssetInstanceCollection* TargetTransaction = TargetInterchange->GetInstanceCollection(TargetSourceId);
	if (!TargetTransaction)
	{
		Fail(TEXT("Failed to get transaction source"));
		return;
	}

	if (!TargetTransaction->AddInstance(TargetAssetId, TargetQuantity * ClaimQuantity))
	{
		Fail(TEXT("Failed to add item"));
		return;
	}

	Success();
}

