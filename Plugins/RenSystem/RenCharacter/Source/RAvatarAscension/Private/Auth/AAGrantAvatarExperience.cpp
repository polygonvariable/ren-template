// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Auth/AAGrantAvatarExperience.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Core/AscensionLibrary.h"
#include "Core/AssetInstanceUtil.h"
#include "Core/AssetManagerUtil.h"
#include "Core/Type/AssetDetail.h"
#include "Core/Type/Runtime/AvatarInstance.h"
#include "Data/AscensionAsset.h"
#include "Data/AscensionFragment.h"
#include "Data/AssetCollection.h"
#include "Data/AvatarAsset.h"
#include "Data/CoreDataAsset.h"
#include "System/AvatarStorageManager.h"
#include "System/AvatarSubsystem.h"


void UAAGrantAvatarExperience::OnStarted()
{
	AssetManager = UAssetManager::GetIfInitialized();

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

	Step_LoadAssets();
}

void UAAGrantAvatarExperience::OnCompleted(bool bSuccess)
{
	FAssetManagerUtil::CancelHandle(_AssetHandle);
	FAssetManagerUtil::CancelHandle(_BreakdownHandle);
}

void UAAGrantAvatarExperience::OnCleanup()
{
	TargetSourceId = TEXT_EMPTY;
	MaterialSourceId = TEXT_EMPTY;
	MaterialId.Invalidate();

	TargetAssetId = FPrimaryAssetId();
	MaterialAssetId = FPrimaryAssetId();

	MaterialQuantity = 0;

	TargetAsset = nullptr;
	MaterialAsset = nullptr;
	AssetManager = nullptr;
	StorageManager = nullptr;
}

void UAAGrantAvatarExperience::Step_LoadAssets()
{
	if (!IsValid(AssetManager))
	{
		Fail(TEXT("AssetManager is invalid"));
		return;
	}

	TArray<FPrimaryAssetId> Assets;
	Assets.Add(TargetAssetId);
	Assets.Add(MaterialAssetId);

	FAssetManagerUtil::CancelHandle(_AssetHandle);

	_AssetHandle = AssetManager->LoadPrimaryAssets(Assets, TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UAAGrantAvatarExperience::Step_HandleOnAssetLoaded));
}

void UAAGrantAvatarExperience::Step_HandleOnAssetLoaded()
{
	FAssetManagerUtil::ReleaseHandle(_AssetHandle);

	TargetAsset = AssetManager->GetPrimaryAssetObject<UAvatarAsset>(TargetAssetId);
	MaterialAsset = AssetManager->GetPrimaryAssetObject<UAvatarAsset>(MaterialAssetId);

	Step_CheckItemAsset();
}

void UAAGrantAvatarExperience::Step_CheckItemAsset()
{
	const FAvatarInstance* AvatarInstance = StorageManager->GetInstance(TargetAssetId);
	const UAscensionFragment* AscensionFragment = TargetAsset->FindFragmentByClass<UAscensionFragment>();
	if (!AvatarInstance || !IsValid(AscensionFragment))
	{
		Fail(TEXT("Invalid avatar instance or asset doesn't support ascension (AscensionFragment is not added)"));
		return;
	}

	AscensionData = AvatarInstance->Ascension;

	ExperiencePerLevel = AscensionFragment->GetExperienceInterval(AscensionData.Level);
	LevelPerRank = AscensionFragment->GetLevelInterval(AscensionData.Rank);
	MaxLevel = AscensionFragment->GetMaxLevel();
	MaxRank = AscensionFragment->GetMaxRank();

	const UAssetCollection* ExperienceCollection = AscensionFragment->GetExperienceAssets(AscensionData);
	if (!IsValid(ExperienceCollection))
	{
		Fail(TEXT("Failed to get level up collection"));
		return;
	}

	FAssetDetail MaterialDetail;
	if (!ExperienceCollection->GetAssetDetail(MaterialAssetId, MaterialDetail))
	{
		Fail(TEXT("Material cannot be used for level up"));
		return;
	}

	MaterialQuantity = MaterialDetail.Quantity;

	const FGuid& ExperienceCollectionId = ExperienceCollection->GetCollectionId();

	Step_CheckMaterialAsset(ExperienceCollectionId);
}

void UAAGrantAvatarExperience::Step_CheckMaterialAsset(const FGuid& ExperienceCollectionId)
{
	if (!IsValid(MaterialAsset))
	{
		Fail(TEXT("MaterialAsset is invalid"));
		return;
	}

	// const IAssetCompositionInterface* MaterialComposition = Cast<IAssetCompositionInterface>(MaterialAsset);
	// if (!MaterialComposition)
	// {
	// 	Fail(TEXT("Material asset doesn't have asset structure"));
	// 	return;
	// }

	// // Possible items that material can break into
	// // in this case the Material item will break into Exp item
	// const UAssetCollection* BreakdownCollection = MaterialComposition->GetBreakdownAssets(ExperienceCollectionId);
	// if (!IsValid(BreakdownCollection))
	// {
	// 	Fail(TEXT("Invalid BreakdownAssets"));
	// 	return;
	// }

	// TPair<FPrimaryAssetId, FAssetDetail> Collection;
	// if (!BreakdownCollection->GetRandomAsset(Collection))
	// {
	// 	Fail(TEXT("Failed to get asset pair"));
	// 	return;
	// }

	// const FPrimaryAssetId& BreakdownAssetId = Collection.Key;
	// int BreakdownQuantity = Collection.Value.Quantity;

	// Step_LoadBreakdownAsset(BreakdownAssetId, BreakdownQuantity);
}

void UAAGrantAvatarExperience::Step_LoadBreakdownAsset(const FPrimaryAssetId& AssetId, int Quantity)
{
	FAssetManagerUtil::CancelHandle(_BreakdownHandle);

	_BreakdownHandle = AssetManager->LoadPrimaryAsset(AssetId, TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UAAGrantAvatarExperience::Step_HandleOnBreakdownAssetLoaded, AssetId, Quantity));
}

void UAAGrantAvatarExperience::Step_HandleOnBreakdownAssetLoaded(FPrimaryAssetId AssetId, int Quantity)
{
	FAssetManagerUtil::ReleaseHandle(_BreakdownHandle);

	const UExperiencePointAsset* PointAsset = AssetManager->GetPrimaryAssetObject<UExperiencePointAsset>(AssetId);

	Points = PointAsset->GetPoints(Quantity);
	Step_RemoveMaterial();
}

void UAAGrantAvatarExperience::Step_RemoveMaterial()
{
	IAssetInstanceCollectionProvider* MaterialProvider = FAssetInstanceUtil::GetInstanceCollectionProvider(GetWorld(), MaterialAssetId);
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

	bool bRemoved = MaterialCollection->RemoveInstance(MaterialAssetId, MaterialQuantity);
	if (!bRemoved)
	{
		Fail(TEXT("Failed to remove material"));
		return;
	}

	Step_AddExperience();
}

void UAAGrantAvatarExperience::Step_AddExperience()
{
	int Amount = FMath::Max(0, (Points * MaterialQuantity));

	int NewExperience = 0;
	int NewLevel = 0;

	if (!FAscensionLibrary::AddExperience(AscensionData, Amount, ExperiencePerLevel, LevelPerRank, MaxLevel, MaxRank, NewExperience, NewLevel))
	{
		Fail(TEXT("Failed to add experience"));
		return;
	}

	bool bSuccess = StorageManager->UpdateInstance(TargetAssetId, [NewExperience, NewLevel](FAvatarInstance* Instance)
		{
			if (Instance)
			{
				Instance->Ascension.Experience = NewExperience;
				Instance->Ascension.Level = NewLevel;
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

