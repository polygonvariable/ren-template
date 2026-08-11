// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Auth/AACraftClaimItem.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Core/Type/Runtime/TradeKey.h"
#include "Data/TradeAsset.h"
#include "Data/TradeAssetCollection.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetRuleDefinition.h"
#include "Interface/AssetInstanceCollection.h"
#include "Interface/AssetInstanceCollectionProvider.h"
#include "Library/AssetInstanceUtil.h"
#include "Library/AssetManagerUtil.h"
#include "Management/AssetGroup.h"
#include "System/CraftStorageManager.h"
#include "System/CraftSubsystem.h"


void UAACraftClaimItem::OnStarted()
{
	AssetManager = UAssetManager::GetIfInitialized();
	Step_LoadAsset();
}

void UAACraftClaimItem::OnCompleted(bool bSuccess)
{
	FAssetManagerUtil::CancelHandle(_AssetHandle);
}

void UAACraftClaimItem::OnCleanup()
{
	AssetManager = nullptr;
	TradeAsset = nullptr;

	CraftAssetId = FPrimaryAssetId();
	TargetAssetId = FPrimaryAssetId();

	TargetQuantity = 0;
	ClaimQuantity = 0;
}

void UAACraftClaimItem::Step_LoadAsset()
{
	if (!IsValid(AssetManager))
	{
		Fail(TEXT("AssetManager is invalid"));
		return;
	}

	FAssetManagerUtil::CancelHandle(_AssetHandle);

	_AssetHandle = AssetManager->LoadPrimaryAsset(CraftAssetId, TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UAACraftClaimItem::Step_HandleOnAssetsLoaded));
}

void UAACraftClaimItem::Step_HandleOnAssetsLoaded()
{
	FAssetManagerUtil::ReleaseHandle(_AssetHandle);

	TradeAsset = AssetManager->GetPrimaryAssetObject<UTradeAsset>(CraftAssetId);

	Step_CheckTarget();
}

void UAACraftClaimItem::Step_CheckTarget()
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

	Step_CheckClaimable();
}

void UAACraftClaimItem::Step_CheckClaimable()
{
	UWorld* World = GetWorld();
	UGameInstance* GameInstance = World->GetGameInstance();

	UCraftSubsystem* CraftSubsystem = UCraftSubsystem::Get(GameInstance);
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

	ClaimQuantity = StorageManager->ClaimCraftedItems(FTradeKey(CraftAssetId, TradeCollectionId, TargetAssetId));
	if (ClaimQuantity <= 0)
	{
		Fail(TEXT("No item to claim"));
		return;
	}

	Step_PerformTransaction();
}

void UAACraftClaimItem::Step_PerformTransaction()
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

