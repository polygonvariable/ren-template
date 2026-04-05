// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/Task_GrantItemExperience.h"

// Engine Headers

// Project Headers
#include "Asset/AscensionAsset.h"
#include "Asset/InventoryAsset.h"
#include "Asset/CoreDataAsset.h"
#include "Definition/Runtime/InventoryInstance.h"
#include "Interface/AscensionProvider.h"
#include "Interface/AssetComposition.h"
#include "Library/AscensionLibrary.h"
#include "Management/Collection/AssetCollection_Simple.h"
#include "Manager/RAssetManager.inl"
#include "Storage/InventoryStorage.h"
#include "Subsystem/InventorySubsystem.h"




void UTask_GrantItemExperience::OnStarted()
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

	Step_LoadAssets();
}

void UTask_GrantItemExperience::OnStopped()
{
	AssetManager->CancelFetch(TaskId);
}

void UTask_GrantItemExperience::OnCleanup()
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
	Inventory = nullptr;
}

void UTask_GrantItemExperience::Step_LoadAssets()
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

	TWeakObjectPtr<UTask_GrantItemExperience> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAssets<UCoreDataAsset>& Result)
		{
			UTask_GrantItemExperience* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			const TArray<UCoreDataAsset*>& Assets = Result.Get();
			This->TargetAsset = Cast<UInventoryAsset>(Assets[0]);
			This->MaterialAsset = Assets[1];

			This->Step_CheckItemAsset();
		}
	);
}

void UTask_GrantItemExperience::Step_CheckItemAsset()
{
	const FInventoryInstance* Item = Inventory->GetInstanceById(TargetAssetId, TargetId);
	if (!Item)
	{
		Fail(TEXT("Item not found, TargetAsset is invalid"));
		return;
	}

	const IAscensionProvider* AscensionProvider = Cast<IAscensionProvider>(TargetAsset);
	if (!AscensionProvider)
	{
		Fail(TEXT("The item doesn't support ascension (IAscensionProvider is not implemented)"));
		return;
	}

	AscensionData = Item->Ascension;

	ExperiencePerLevel = AscensionProvider->GetExperienceInterval(AscensionData.Level);
	LevelPerRank = AscensionProvider->GetLevelInterval(AscensionData.Rank);
	MaxLevel = AscensionProvider->GetMaxLevel();
	MaxRank = AscensionProvider->GetMaxRank();

	const UAssetCollection* ExperienceItems = AscensionProvider->GetExperienceAssets(AscensionData);
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

void UTask_GrantItemExperience::Step_CheckMaterialAsset(const FGuid& MaterialCollectionId)
{
	if (!IsValid(MaterialAsset) || !IsValid(Inventory))
	{
		Fail(TEXT("MaterialAsset, Inventory is invalid"));
		return;
	}

	const IAssetCompositionInterface* MaterialStructure = Cast<IAssetCompositionInterface>(MaterialAsset);
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

void UTask_GrantItemExperience::Step_LoadBreakdownAsset(const FPrimaryAssetId& AssetId, int Quantity)
{
	TFuture<FLatentLoadedAsset<UExperiencePointAsset>> Future = AssetManager->FetchPrimaryAsset<UExperiencePointAsset>(TaskId, AssetId);
	if (!Future.IsValid())
	{
		Fail(TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UTask_GrantItemExperience> WeakThis(this);
	Future.Next([WeakThis, Quantity](const FLatentLoadedAsset<UExperiencePointAsset>& Result)
		{
			UTask_GrantItemExperience* This = WeakThis.Get();
			if (!IsValid(This) || !Result.IsValid())
			{
				This->Fail(TEXT("Failed to fetch assets"));
				return;
			}

			const UExperiencePointAsset* PointAsset = Result.Get();
			
			This->Points = PointAsset->GetPoints(Quantity);
			This->Step_RemoveItem();
		}
	);
}

void UTask_GrantItemExperience::Step_RemoveItem()
{
	bool bRemoved = Inventory->RemoveInstanceById(MaterialAssetId, MaterialId, MaterialQuantity);
	if (!bRemoved)
	{
		Fail(TEXT("Failed to remove material"));
		return;
	}

	Step_AddExperience();
}

// UE_DISABLE_OPTIMIZATION
void UTask_GrantItemExperience::Step_AddExperience()
{
	const FInventoryInstance* Item = Inventory->GetInstanceById(TargetAssetId, TargetId);
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

	bool bSuccess = Inventory->UpdateInstanceById(TargetAssetId, TargetId, [NewExperience, NewLevel](FInventoryInstance* Item)
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
// UE_ENABLE_OPTIMIZATION

