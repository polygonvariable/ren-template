// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Auth/AAGrantItemRank.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Core/AscensionLibrary.h"
#include "Core/AssetManagerUtil.h"
#include "Core/Type/AscensionData.h"
#include "Core/Type/Runtime/InventoryInstance.h"
#include "Data/AscensionFragment.h"
#include "Data/AssetCollection.h"
#include "Data/CoreDataAsset.h"
#include "Data/InventoryAsset.h"
#include "System/InventoryStorageManager.h"
#include "System/InventorySubsystem.h"


void UAAGrantItemRank::OnStarted()
{
	AssetManager = UAssetManager::GetIfInitialized();

	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(GetWorld());
	if (!IsValid(AssetManager) || !IsValid(InventorySubsystem))
	{
		Fail(TEXT("AssetManager, InventorySubsystem is invalid"));
		return;
	}

	StorageManager = InventorySubsystem->GetStorageManager(SourceId);
	if (!IsValid(StorageManager))
	{
		Fail(TEXT("InventoryStorage is invalid"));
		return;
	}

	Step_LoadAsset();
}

void UAAGrantItemRank::OnCompleted(bool bSuccess)
{
	FAssetManagerUtil::CancelHandle(_AssetHandle);
}

void UAAGrantItemRank::OnCleanup()
{
	SourceId = TEXT_EMPTY;
	TargetId.Invalidate();

	TargetAssetId = FPrimaryAssetId();

	TargetAsset = nullptr;
	AssetManager = nullptr;
	StorageManager = nullptr;
}

void UAAGrantItemRank::Step_LoadAsset()
{
	FAssetManagerUtil::CancelHandle(_AssetHandle);

	_AssetHandle = AssetManager->LoadPrimaryAsset(TargetAssetId, TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UAAGrantItemRank::Step_HandleOnAssetLoaded));
}

void UAAGrantItemRank::Step_HandleOnAssetLoaded()
{
	FAssetManagerUtil::ReleaseHandle(_AssetHandle);

	TargetAsset = AssetManager->GetPrimaryAssetObject<UInventoryAsset>(TargetAssetId);
	Step_CheckTarget();
}

void UAAGrantItemRank::Step_CheckTarget()
{
	const FInventoryInstance* InventoryInstance = StorageManager->GetInstanceById(TargetAssetId, TargetId);
	const UAscensionFragment* AscensionFragment = TargetAsset->FindFragmentByClass<UAscensionFragment>();
	if (!InventoryInstance || !IsValid(AscensionFragment))
	{
		Fail(TEXT("InventoryInstance, AscensionFragment is invalid"));
		return;
	}

	const FAscensionData& AscensionData = InventoryInstance->Ascension;

	int LevelPerRank = AscensionFragment->GetLevelInterval(AscensionData.Rank);
	int MaxLevel = AscensionFragment->GetMaxLevel();
	int MaxRank = AscensionFragment->GetMaxRank();

	bool bRankUpRequired = FAscensionLibrary::IsRankUpRequired(AscensionData, LevelPerRank, MaxLevel, MaxRank);
	if (!bRankUpRequired)
	{
		Fail(TEXT("Item cannot rank up"));
		return;
	}

	const UAssetCollection* RankItems = AscensionFragment->GetRankAssets(AscensionData);
	if (!IsValid(RankItems))
	{
		Fail(TEXT("The material cannot be used to upgrade the item"));
		return;
	}

	TMap<FPrimaryAssetId, int> MaterialsToRemove;
	RankItems->GetAssetList(MaterialsToRemove);

	bool bRemoved = StorageManager->RemoveInstances(MaterialsToRemove, 1);
	if (!bRemoved)
	{
		Fail(TEXT("Failed to remove material"));
		return;
	}

	bool bSuccess = StorageManager->UpdateInstanceById(TargetAssetId, TargetId,
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

