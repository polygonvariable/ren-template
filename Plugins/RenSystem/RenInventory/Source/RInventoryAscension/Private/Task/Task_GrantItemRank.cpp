// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/Task_GrantItemRank.h"

// Engine Headers

// Project Headers
#include "Asset/InventoryAsset.h"
#include "Asset/CoreDataAsset.h"
#include "Definition/Runtime/InventoryInstance.h"
#include "Interface/AscensionProvider.h"
#include "Library/AscensionLibrary.h"
#include "Management/Collection/AssetCollection_Simple.h"
#include "Manager/RAssetManager.inl"
#include "Storage/InventoryStorage.h"
#include "Subsystem/InventorySubsystem.h"




void UTask_GrantItemRank::OnStarted()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(GetWorld());
	if (!IsValid(AssetManager) || !IsValid(InventorySubsystem))
	{
		Fail(TEXT("AssetManager, InventorySubsystem is invalid"));
		return;
	}

	UInventoryStorage* InventoryStorage = InventorySubsystem->GetInventory(SourceId);
	if (!IsValid(InventoryStorage))
	{
		Fail(TEXT("InventoryStorage is invalid"));
		return;
	}

	Inventory = InventoryStorage;

	Step_LoadAsset();
}

void UTask_GrantItemRank::OnStopped()
{
	AssetManager->CancelFetch(TaskId);
}

void UTask_GrantItemRank::OnCleanup()
{
	SourceId = TEXT_EMPTY;
	TargetId.Invalidate();

	TargetAssetId = FPrimaryAssetId();

	TargetAsset = nullptr;
	AssetManager = nullptr;
	Inventory = nullptr;

	AscensionData.Reset();
}

void UTask_GrantItemRank::Step_LoadAsset()
{
	TFuture<FLatentLoadedAsset<UCoreDataAsset>> Future = AssetManager->FetchPrimaryAsset<UCoreDataAsset>(FGuid::NewGuid(), TargetAssetId);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UTask_GrantItemRank> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAsset<UCoreDataAsset>& Result)
		{
			UTask_GrantItemRank* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			This->TargetAsset = Cast<UInventoryAsset>(Result.Get());
			This->Step_CheckTarget();
		}
	);
}

void UTask_GrantItemRank::Step_CheckTarget()
{
	const IAscensionProvider* AscensionProvider = Cast<IAscensionProvider>(TargetAsset);
	if (!AscensionProvider)
	{
		Fail(TEXT("The item doesn't support ascension (IAscensionProvider is not implemented)"));
		return;
	}

	const FInventoryInstance* Item = Inventory->GetInstanceById(TargetAssetId, TargetId);
	if (!IsValid(TargetAsset) || !Item)
	{
		Fail(TEXT("Item not found, TargetAsset is invalid"));
		return;
	}

	AscensionData = Item->Ascension;

	ExperiencePerLevel = AscensionProvider->GetExperienceInterval(AscensionData.Level);
	LevelPerRank = AscensionProvider->GetLevelInterval(AscensionData.Rank);
	MaxLevel = AscensionProvider->GetMaxLevel();
	MaxRank = AscensionProvider->GetMaxRank();

	bool bRankUpRequired = UAscensionLibrary::IsRankUpRequired(AscensionData, LevelPerRank, MaxLevel, MaxRank);
	if (!bRankUpRequired)
	{
		Fail(TEXT("Item cannot rank up"));
		return;
	}

	const UAssetCollection* RankItems = AscensionProvider->GetRankAssets(AscensionData);
	if (!IsValid(RankItems))
	{
		Fail(TEXT("The material cannot be used to upgrade the item"));
		return;
	}

	TMap<FPrimaryAssetId, int> AssetList;
	RankItems->GetAssetList(AssetList);

	bool bRemoved = Inventory->RemoveInstances(AssetList, 1);
	if (!bRemoved)
	{
		Fail(TEXT("Failed to remove material"));
		return;
	}

	bool bSuccess = Inventory->UpdateInstanceById(TargetAssetId, TargetId,
		[](FInventoryInstance* Item)
		{
			if (Item)
			{
				Item->Ascension.Rank++;
				Item->Sanitize();
			}
		}
	);

	if (!bSuccess)
	{
		Fail(TEXT("Failed to update"));
		return;
	}

	Success();
}

