// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Auth/AAGiveReward.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Asset/RewardAsset.h"
#include "Core/Interface/AssetInstanceCollection.h"
#include "Core/Interface/AssetInstanceCollectionProvider.h"
#include "Core/AssetInstanceUtil.h"
#include "Core/AssetManagerUtil.h"
#include "Data/AssetCollection.h"
#include "Settings/RewardSettings.h"
#include "Storage/RewardStorageManager.h"
#include "Subsystem/RewardSubsystem.h"


void UAAGiveReward::OnStarted()
{
	RewardSubsystem = URewardSubsystem::Get(GetWorld());
	AssetManager = UAssetManager::GetIfInitialized();

	Step_LoadReward();
}

void UAAGiveReward::OnCleanup()
{
	RewardSubsystem = nullptr;
	RewardAssetId = FPrimaryAssetId();

	FAssetManagerUtil::CancelHandle(RewardHandle);
}

void UAAGiveReward::Step_LoadReward()
{
	RewardHandle = AssetManager->LoadPrimaryAsset(RewardAssetId, TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UAAGiveReward::Step_OnRewardLoaded));
}

void UAAGiveReward::Step_OnRewardLoaded()
{
	FAssetManagerUtil::ReleaseHandle(RewardHandle);

	const URewardAsset* Asset = AssetManager->GetPrimaryAssetObject<URewardAsset>(RewardAssetId);
	if (!IsValid(Asset))
	{
		Fail(TEXT("Failed to load reward asset"));
		return;
	}

	const UAssetCollection* Collection = Asset->AssetCollection;
	if (!IsValid(Collection))
	{
		Fail(TEXT("Failed to load reward collection"));
		return;
	}

	Step_AddReward(Collection);
}

void UAAGiveReward::Step_AddReward(const UAssetCollection* Collection)
{
	FPrimaryAssetType RewardType = Collection->GetCollectionType();

	TMap<FPrimaryAssetId, int> AssetMap;
	Collection->GetAssetList(AssetMap);

	IAssetInstanceCollection* AssetInstance = FAssetInstanceUtil::GetPrimaryInstanceCollection(GetWorld(), RewardType);
	if (!AssetInstance)
	{
		Fail(TEXT("Failed to load reward provider"));
		return;
	}

	bool bSuccess = AssetInstance->AddInstances(AssetMap, 1);
	if (!bSuccess)
	{
		Fail(TEXT("Failed to add reward instances"));
		return;
	}

	Step_Finalize();
}

void UAAGiveReward::Step_Finalize()
{
	Success();
}

