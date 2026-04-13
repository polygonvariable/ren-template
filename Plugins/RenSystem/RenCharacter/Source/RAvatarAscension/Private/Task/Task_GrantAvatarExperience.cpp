// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/Task_GrantAvatarExperience.h"

// Engine Headers

// Project Headers
#include "Asset/AscensionAsset.h"
#include "Asset/AvatarAsset.h"
#include "Asset/CoreDataAsset.h"
#include "Definition/Runtime/AvatarInstance.h"
#include "Interface/AscensionProvider.h"
#include "Interface/AssetComposition.h"
#include "Library/AscensionLibrary.h"
#include "Management/Collection/AssetCollection_Simple.h"
#include "Manager/RAssetManager.inl"
#include "Storage/AvatarStorage.h"
#include "Subsystem/AvatarSubsystem.h"
#include "Library/AssetInstanceUtil.h"




void UTask_GrantAvatarExperience::OnStarted()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	UAvatarSubsystem* AvatarSubsystem = UAvatarSubsystem::Get(GetWorld());
	if (!IsValid(AssetManager) || !IsValid(AvatarSubsystem))
	{
		Fail(TEXT("AssetManager, AvatarSubsystem is invalid"));
		return;
	}

	UAvatarStorage* AvatarCollection = AvatarSubsystem->GetAvatarStorage();
	if (!IsValid(AvatarCollection))
	{
		Fail(TEXT("AvatarStorage is invalid"));
		return;
	}

	AvatarStorage = AvatarCollection;

#if WITH_EDITOR
	bool bSuccess = AvatarStorage->UpdateInstance(TargetAssetId, [](FAvatarInstance* Instance)
		{
			if (Instance)
			{
				Instance->Ascension.Level++;
				Instance->Sanitize();
			}
		}
	);

	Success();
	return;
#endif

	// Step_LoadAssets();
}

void UTask_GrantAvatarExperience::OnStopped()
{
	AssetManager->CancelFetch(TaskId);
}

void UTask_GrantAvatarExperience::OnCleanup()
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
	AvatarStorage = nullptr;
}

void UTask_GrantAvatarExperience::Step_LoadAssets()
{
	if (!IsValid(AssetManager))
	{
		Fail(TEXT("AssetManager is invalid"));
		return;
	}

	TArray<FPrimaryAssetId> Assets;
	Assets.Add(TargetAssetId);
	Assets.Add(MaterialAssetId);

	TFuture<FLatentLoadedAssets<UCoreDataAsset>> Future = AssetManager->FetchPrimaryAssets<UCoreDataAsset>(TaskId, Assets);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UTask_GrantAvatarExperience> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAssets<UCoreDataAsset>& Result)
		{
			UTask_GrantAvatarExperience* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			const TArray<UCoreDataAsset*>& Assets = Result.Get();
			This->TargetAsset = Cast<UAvatarAsset>(Assets[0]);
			This->MaterialAsset = Assets[1];

			This->Step_CheckItemAsset();
		}
	);
}

void UTask_GrantAvatarExperience::Step_CheckItemAsset()
{
	const FAvatarInstance* Instance = AvatarStorage->GetInstance(TargetAssetId);
	if (!Instance)
	{
		Fail(TEXT("Instance not found"));
		return;
	}

	const IAscensionProvider* AscensionProvider = Cast<IAscensionProvider>(TargetAsset);
	if (!AscensionProvider)
	{
		Fail(TEXT("The asset doesn't support ascension (IAscensionProvider is not implemented)"));
		return;
	}

	AscensionData = Instance->Ascension;

	ExperiencePerLevel = AscensionProvider->GetExperienceInterval(AscensionData.Level);
	LevelPerRank = AscensionProvider->GetLevelInterval(AscensionData.Rank);
	MaxLevel = AscensionProvider->GetMaxLevel();
	MaxRank = AscensionProvider->GetMaxRank();

	const UAssetCollection* ExperienceCollection = AscensionProvider->GetExperienceAssets(AscensionData);
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

void UTask_GrantAvatarExperience::Step_CheckMaterialAsset(const FGuid& ExperienceCollectionId)
{
	if (!IsValid(MaterialAsset))
	{
		Fail(TEXT("MaterialAsset is invalid"));
		return;
	}

	const IAssetCompositionInterface* MaterialComposition = Cast<IAssetCompositionInterface>(MaterialAsset);
	if (!MaterialComposition)
	{
		Fail(TEXT("Material asset doesn't have asset structure"));
		return;
	}

	// Possible items that material can break into
	// in this case the Material item will break into Exp item
	const UAssetCollection* BreakdownCollection = MaterialComposition->GetBreakdownAssets(ExperienceCollectionId);
	if (!IsValid(BreakdownCollection))
	{
		Fail(TEXT("Invalid BreakdownAssets"));
		return;
	}

	TPair<FPrimaryAssetId, FAssetDetail> Collection;
	if (!BreakdownCollection->GetRandomAsset(Collection))
	{
		Fail(TEXT("Failed to get asset pair"));
		return;
	}

	const FPrimaryAssetId& BreakdownAssetId = Collection.Key;
	int BreakdownQuantity = Collection.Value.Quantity;

	Step_LoadBreakdownAsset(BreakdownAssetId, BreakdownQuantity);
}

void UTask_GrantAvatarExperience::Step_LoadBreakdownAsset(const FPrimaryAssetId& AssetId, int Quantity)
{
	TFuture<FLatentLoadedAsset<UExperiencePointAsset>> Future = AssetManager->FetchPrimaryAsset<UExperiencePointAsset>(TaskId, AssetId);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UTask_GrantAvatarExperience> WeakThis(this);
	Future.Next([WeakThis, Quantity](const FLatentLoadedAsset<UExperiencePointAsset>& Result)
		{
			UTask_GrantAvatarExperience* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			const UExperiencePointAsset* PointAsset = Result.Get();
			
			This->Points = PointAsset->GetPoints(Quantity);
			This->Step_RemoveMaterial();
		}
	);
}

void UTask_GrantAvatarExperience::Step_RemoveMaterial()
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

// UE_DISABLE_OPTIMIZATION
void UTask_GrantAvatarExperience::Step_AddExperience()
{
	int Amount = FMath::Max(0, (Points * MaterialQuantity));

	int NewExperience = 0;
	int NewLevel = 0;

	if (!FAscensionLibrary::AddExperience(AscensionData, Amount, ExperiencePerLevel, LevelPerRank, MaxLevel, MaxRank, NewExperience, NewLevel))
	{
		Fail(TEXT("Failed to add experience"));
		return;
	}

	bool bSuccess = AvatarStorage->UpdateInstance(TargetAssetId, [NewExperience, NewLevel](FAvatarInstance* Instance)
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
// UE_ENABLE_OPTIMIZATION

