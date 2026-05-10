// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/Task_GrantAvatarRank.h"

// Project Headers
#include "Asset/AvatarAsset.h"
#include "Asset/CoreDataAsset.h"
#include "Definition/Runtime/AvatarInstance.h"
#include "Interface/AscensionProvider.h"
#include "Library/AscensionLibrary.h"
#include "Library/AssetInstanceUtil.h"
#include "Management/Collection/AssetCollection_Simple.h"
#include "Manager/RAssetManager.inl"
#include "Storage/AvatarStorageManager.h"
#include "Subsystem/AvatarSubsystem.h"


void UTask_GrantAvatarRank::OnStarted()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	UAvatarSubsystem* AvatarSubsystem = UAvatarSubsystem::Get(GetWorld());
	if (!IsValid(AssetManager) || !IsValid(AvatarSubsystem))
	{
		Fail(TEXT("AssetManager, AvatarSubsystem is invalid"));
		return;
	}

	StorageManager = AvatarSubsystem->GetStorageManager();
	if (!IsValid(StorageManager))
	{
		Fail(TEXT("AvatarStorage is invalid"));
		return;
	}

	Step_LoadAsset();
}

void UTask_GrantAvatarRank::OnCompleted(bool bSuccess)
{
	AssetManager->CancelFetch(ActionId);
}

void UTask_GrantAvatarRank::OnCleanup()
{
	TargetSourceId = TEXT_EMPTY;
	MaterialSourceId = TEXT_EMPTY;

	TargetAssetId = FPrimaryAssetId();

	TargetAsset = nullptr;
	AssetManager = nullptr;
	StorageManager = nullptr;

	AscensionData.Reset();
}

void UTask_GrantAvatarRank::Step_LoadAsset()
{
	TFuture<FLatentLoadedAsset<UCoreDataAsset>> Future = AssetManager->FetchPrimaryAsset<UCoreDataAsset>(FGuid::NewGuid(), TargetAssetId);
	TWeakObjectPtr<UTask_GrantAvatarRank> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAsset<UCoreDataAsset>& Result)
		{
			UTask_GrantAvatarRank* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			This->TargetAsset = Cast<UAvatarAsset>(Result.Get());
			This->Step_CheckTarget();
		}
	);
}

void UTask_GrantAvatarRank::Step_CheckTarget()
{
	const IAscensionProvider* AscensionProvider = Cast<IAscensionProvider>(TargetAsset);
	if (!AscensionProvider)
	{
		Fail(TEXT("The asset doesn't support ascension (IAscensionProvider is not implemented)"));
		return;
	}

	const FAvatarInstance* Instance = StorageManager->GetInstance(TargetAssetId);
	if (!IsValid(TargetAsset) || !Instance)
	{
		Fail(TEXT("Instance not found, TargetAsset is invalid"));
		return;
	}

	AscensionData = Instance->Ascension;

	ExperiencePerLevel = AscensionProvider->GetExperienceInterval(AscensionData.Level);
	LevelPerRank = AscensionProvider->GetLevelInterval(AscensionData.Rank);
	MaxLevel = AscensionProvider->GetMaxLevel();
	MaxRank = AscensionProvider->GetMaxRank();

	if (!FAscensionLibrary::IsRankUpRequired(AscensionData, LevelPerRank, MaxLevel, MaxRank))
	{
		Fail(TEXT("Cannot rank up"));
		return;
	}

	const UAssetCollection* RankCollection = AscensionProvider->GetRankAssets(AscensionData);
	if (!IsValid(RankCollection))
	{
		Fail(TEXT("Failed to get rank up collection"));
		return;
	}

	TMap<FPrimaryAssetId, int> RankAssetList;
	RankCollection->GetAssetList(RankAssetList);
	
	FPrimaryAssetType RankAssetType = RankCollection->GetCollectionType();

	Step_RemoveMaterial(RankAssetList, RankAssetType);
}

void UTask_GrantAvatarRank::Step_RemoveMaterial(const TMap<FPrimaryAssetId, int>& Materials, FPrimaryAssetType MaterialType)
{
	IAssetInstanceCollectionProvider* MaterialProvider = FAssetInstanceUtil::GetInstanceCollectionProvider(GetWorld(), MaterialType);
	if (!MaterialProvider)
	{
		Fail(TEXT("Failed to get instance collection provider"));
		return;
	}

	FName MaterialCollectionId = MaterialProvider->GetPrimaryCollectionId();
	IAssetInstanceCollection* MaterialCollection = MaterialProvider->GetInstanceCollection(MaterialCollectionId);
	if (!MaterialCollection)
	{
		Fail(TEXT("Failed to get instance collection"));
		return;
	}

	bool bRemoved = MaterialCollection->RemoveInstances(Materials, 1);
	if (!bRemoved)
	{
		Fail(TEXT("Failed to remove material"));
		return;
	}

	Step_AddRank();
}

void UTask_GrantAvatarRank::Step_AddRank()
{
	bool bSuccess = StorageManager->UpdateInstance(TargetAssetId, [](FAvatarInstance* Instance)
		{
			if (Instance)
			{
				Instance->Ascension.Rank++;
				Instance->Sanitize();
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

