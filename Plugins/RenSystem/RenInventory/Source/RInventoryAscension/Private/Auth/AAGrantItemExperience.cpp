// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Auth/AAGrantItemExperience.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Core/AscensionLibrary.h"
#include "Core/AssetManagerUtil.h"
#include "Core/Type/AssetDetail.h"
#include "Core/Type/Runtime/InventoryInstance.h"
#include "Data/AscensionFragment.h"
#include "Data/AssetCollection.h"
#include "Data/CoreDataAsset.h"
#include "Data/Fragment/AssetCompositionFragment.h"
#include "Data/InventoryAsset.h"
#include "System/InventoryStorageManager.h"
#include "System/InventorySubsystem.h"


void UAAGrantItemExperience::OnStarted()
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

	Step_LoadAssets();
}

void UAAGrantItemExperience::OnCompleted(bool bSuccess)
{
	FAssetManagerUtil::CancelHandle(_AssetHandle);
}

void UAAGrantItemExperience::OnCleanup()
{
	SourceId = TEXT_EMPTY;
	TargetId.Invalidate();
	MaterialId.Invalidate();

	TargetAssetId = FPrimaryAssetId();
	MaterialAssetId = FPrimaryAssetId();

	MaterialQuantity = 0;

	TargetAsset = nullptr;
	MaterialAsset = nullptr;
	AssetManager = nullptr;
	StorageManager = nullptr;
}

void UAAGrantItemExperience::Step_LoadAssets()
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

	_AssetHandle = AssetManager->LoadPrimaryAssets(Assets, TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UAAGrantItemExperience::Step_HandleOnAssetLoaded));
}

void UAAGrantItemExperience::Step_HandleOnAssetLoaded()
{
	FAssetManagerUtil::ReleaseHandle(_AssetHandle);

	TargetAsset = AssetManager->GetPrimaryAssetObject<UInventoryAsset>(TargetAssetId);
	MaterialAsset = AssetManager->GetPrimaryAssetObject<UInventoryAsset>(MaterialAssetId);

	Step_CheckItemAsset();
}

void UAAGrantItemExperience::Step_CheckItemAsset()
{
	const FInventoryInstance* InventoryInstance = StorageManager->GetInstanceById(TargetAssetId, TargetId);
	const UAscensionFragment* AscensionFragment = TargetAsset->FindFragmentByClass<UAscensionFragment>();
	if (!InventoryInstance || !IsValid(AscensionFragment))
	{
		Fail(TEXT("InventoryInstance, AscensionFragment is invalid"));
		return;
	}

	AscensionData = InventoryInstance->Ascension;

	ExperiencePerLevel = AscensionFragment->GetExperienceInterval(AscensionData.Level);
	LevelPerRank = AscensionFragment->GetLevelInterval(AscensionData.Rank);
	MaxLevel = AscensionFragment->GetMaxLevel();
	MaxRank = AscensionFragment->GetMaxRank();

	const UAssetCollection* ExperienceItems = AscensionFragment->GetExperienceAssets(AscensionData);
	if (!IsValid(ExperienceItems))
	{
		Fail(TEXT("The material cannot be used to upgrade the item"));
		return;
	}

	const FGuid& MaterialCollectionId = ExperienceItems->GetCollectionId();

	FAssetDetail MaterialDetail;
	if (!ExperienceItems->GetAssetDetail(MaterialAssetId, MaterialDetail))
	{
		Fail(TEXT("Failed to fetch collection item"));
		return;
	}

	MaterialQuantity = MaterialDetail.Quantity;
	
	Step_CheckMaterialAsset(MaterialCollectionId);
}

void UAAGrantItemExperience::Step_CheckMaterialAsset(const FGuid& MaterialCollectionId)
{
	if (!IsValid(MaterialAsset) || !IsValid(StorageManager))
	{
		Fail(TEXT("MaterialAsset, Inventory is invalid"));
		return;
	}

	const UAssetCompositionFragment* MaterialStructure = MaterialAsset->FindFragmentByClass<UAssetCompositionFragment>();
	if (!MaterialStructure)
	{
		Fail(TEXT("Material asset doesn't have asset structure"));
		return;
	}

	// Possible items that material can break into
	// in this case the Material item will break into Exp item
	const UAssetCollection* BreakdownAssets = MaterialStructure->GetBreakdownAssets(MaterialCollectionId);
	if (!IsValid(BreakdownAssets))
	{
		Fail(TEXT("Invalid BreakdownAssets"));
		return;
	}

	TPair<FPrimaryAssetId, FAssetDetail> Collection;
	bool bFound = BreakdownAssets->GetRandomAsset(Collection);
	if (!bFound)
	{
		Fail(TEXT("Failed to get asset pair"));
		return;
	}

	const FPrimaryAssetId& BreakdownAssetId = Collection.Key;
	int BreakdownQuantity = Collection.Value.Quantity;

	Step_LoadBreakdownAsset(BreakdownAssetId, BreakdownQuantity);
}

void UAAGrantItemExperience::Step_LoadBreakdownAsset(const FPrimaryAssetId& AssetId, int Quantity)
{
	//TFuture<FLatentLoadedAsset<UExperiencePointAsset>> Future = AssetManager->FetchPrimaryAsset<UExperiencePointAsset>(ActionId, AssetId);
	//TWeakObjectPtr<UAAGrantItemExperience> WeakThis(this);
	//Future.Next([WeakThis, Quantity](const FLatentLoadedAsset<UExperiencePointAsset>& Result)
	//	{
	//		UAAGrantItemExperience* This = WeakThis.Get();
	//		if (!IsValid(This) || !Result.IsValid())
	//		{
	//			This->Fail(TEXT("Failed to fetch assets"));
	//			return;
	//		}

	//		const UExperiencePointAsset* PointAsset = Result.Get();
	//		
	//		This->Points = PointAsset->GetPoints(Quantity);
	//		This->Step_RemoveItem();
	//	}
	//);
}

void UAAGrantItemExperience::Step_RemoveItem()
{
	bool bRemoved = StorageManager->RemoveInstanceById(MaterialAssetId, MaterialId, MaterialQuantity);
	if (!bRemoved)
	{
		Fail(TEXT("Failed to remove material"));
		return;
	}

	Step_AddExperience();
}

void UAAGrantItemExperience::Step_AddExperience()
{
	const FInventoryInstance* Item = StorageManager->GetInstanceById(TargetAssetId, TargetId);
	if (!Item)
	{
		Fail(TEXT("Item not found"));
		return;
	}

	int Amount = FMath::Max(0, (Points * MaterialQuantity));

	int NewExperience = 0;
	int NewLevel = 0;

	bool bAdded = FAscensionLibrary::AddExperience(AscensionData, Amount, ExperiencePerLevel, LevelPerRank, MaxLevel, MaxRank, NewExperience, NewLevel);
	if (!bAdded)
	{
		Fail(TEXT("Failed to add experience"));
		return;
	}

	bool bSuccess = StorageManager->UpdateInstanceById(TargetAssetId, TargetId, [NewExperience, NewLevel](FInventoryInstance* Item)
		{
			if (Item)
			{
				Item->Ascension.Experience = NewExperience;
				Item->Ascension.Level = NewLevel;
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

