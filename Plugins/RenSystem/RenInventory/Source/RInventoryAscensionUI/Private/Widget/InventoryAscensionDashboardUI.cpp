// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryAscensionDashboardUI.h"

// Engine Headers
#include "Components/Button.h"
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetFilterProperty.h"
#include "Definition/Runtime/InventoryInstance.h"
#include "Delegate/GameUIDelegate.h"
#include "Filter/Criterion/FilterCriterion_Leaf.h"
#include "Interface/AscensionProvider.h"
#include "Library/AscensionLibrary.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetCollection.h"
#include "Storage/InventoryStorageManager.h"
#include "Subsystem/InventoryAscensionSubsystem.h"
#include "Subsystem/InventorySubsystem.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/AssetDetailUI.h"
#include "Widget/AssetEntry.h"
#include "Widget/InventoryEntry.h"


void UInventoryAscensionDashboardUI::InitializeDetail()
{
	InventoryDetail->PrimarySourceId = PrimarySourceId;
	InventoryDetail->InitializeDetail();

	LevelItemCollection->PrimarySourceId = PrimarySourceId;
	LevelItemCollection->InitializeCollection();

	RankItemCollection->PrimarySourceId = PrimarySourceId;
	RankItemCollection->InitializeCollection();

	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(GetGameInstance());
	if (!IsValid(InventorySubsystem))
	{
		return;
	}

	StorageManager = InventorySubsystem->GetStorageManager(PrimarySourceId);
	if (IsValid(StorageManager) && bAutoRefresh)
	{
		StorageManager->OnStorageUpdated.AddUObject(this, &UInventoryAscensionDashboardUI::RefreshDetail);
	}
}

void UInventoryAscensionDashboardUI::RefreshDetail()
{
	if (!IsValid(StorageManager))
	{
		return;
	}

	const FInventoryInstance* Item = StorageManager->GetInstanceById(GetActiveAssetId(), ActiveItemId);
	ToggleAscension(Item);
}


void UInventoryAscensionDashboardUI::ToggleAscension(const FInventoryInstance* Item)
{
	if (!AscensionProvider || !Item)
	{
		return;
	}

	int LevelPerRank = AscensionProvider->GetLevelInterval(Item->Ascension.Rank);
	int MaxLevel = AscensionProvider->GetMaxLevel();
	int MaxRank = AscensionProvider->GetMaxRank();

	if (FAscensionLibrary::IsRankUpRequired(Item->Ascension, LevelPerRank, MaxLevel, MaxRank))
	{
		ToggleRankUp(Item);
	}
	else
	{
		ToggleLevelUp(Item);
	}
}

void UInventoryAscensionDashboardUI::ToggleLevelUp(const FInventoryInstance* Item)
{
	LevelUpButton->SetVisibility(ESlateVisibility::Visible);
	RankUpButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryAscensionDashboardUI::ToggleRankUp(const FInventoryInstance* Item)
{
	LevelUpButton->SetVisibility(ESlateVisibility::Collapsed);
	RankUpButton->SetVisibility(ESlateVisibility::Visible);

	if (!AscensionProvider || !Item)
	{
		return;
	}

	RankItemCollection->ClearSubDetails();

	UFilterCriterion_Asset* AssetFilter = RankItemCollection->GetCriterionByName<UFilterCriterion_Asset>(FAssetFilterProperty::AssetId);
	if (IsValid(AssetFilter))
	{
		AssetFilter->Included.Empty();

		const UAssetCollection* ItemCollection = AscensionProvider->GetRankAssets(Item->Ascension);
		if (IsValid(ItemCollection))
		{
			TMap<FPrimaryAssetId, FAssetDetail> AssetList;
			ItemCollection->GetAssetList(AssetList);

			for (const TPair<FPrimaryAssetId, FAssetDetail>& AssetKv : AssetList)
			{
				const FPrimaryAssetId& AssetId = AssetKv.Key;

				AssetFilter->Included.Add(AssetId);
				RankItemCollection->AddSubDetails(AssetId, FInstancedStruct::Make(AssetKv.Value));
			}
		}
	}

	RankItemCollection->RefreshEntries();
}


void UInventoryAscensionDashboardUI::HandleLevelUp()
{
	const UAssetEntry* Entry = LevelItemCollection->GetSelectedEntry();
	if (!IsValid(AscensionSubsystem) || !IsValid(Entry))
	{
		LOG_ERROR(LogInventoryAscension, TEXT("AscensionSubsystem, Entry is not valid"));
		return;
	}

	FGuid MaterialId = Entry->GetAssetInstanceId();
	FPrimaryAssetId MaterialAssetId = Entry->AssetId;
	
	AscensionSubsystem->TryAddExperiencePoints(PrimarySourceId, GetActiveAssetId(), ActiveItemId, MaterialAssetId, MaterialId);
}

void UInventoryAscensionDashboardUI::HandleRankUp()
{
	if (!IsValid(AscensionSubsystem))
	{
		LOG_ERROR(LogInventoryAscension, TEXT("AscensionSubsystem is not valid"));
		return;
	}

	AscensionSubsystem->TryAddRankPoints(PrimarySourceId, GetActiveAssetId(), ActiveItemId);
}


void UInventoryAscensionDashboardUI::LockControls_Implementation()
{
	LevelUpButton->SetIsEnabled(false);
	RankUpButton->SetIsEnabled(false);
}

void UInventoryAscensionDashboardUI::UnlockControls_Implementation()
{
	LevelUpButton->SetIsEnabled(true);
	RankUpButton->SetIsEnabled(true);
}

void UInventoryAscensionDashboardUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	InventoryDetail->InitializeAssetDetail(Asset);

	AscensionProvider = Cast<IAscensionProvider>(Asset);
	if (AscensionProvider)
	{
		UFilterCriterion_Asset* AssetFilter = LevelItemCollection->GetCriterionByName<UFilterCriterion_Asset>(FAssetFilterProperty::AssetId);
		if (IsValid(AssetFilter))
		{
			AssetFilter->Included.Empty();

			const UAssetCollection* ItemCollection = AscensionProvider->GetExperienceAssets(AscensionInstance);
			if (IsValid(ItemCollection))
			{
				TArray<FPrimaryAssetId> AssetList;
				ItemCollection->GetAssetIds(AssetList);

				AssetFilter->Included.Append(AssetList);
			}
		}

		LevelItemCollection->DisplayEntries();
	}
}

void UInventoryAscensionDashboardUI::SetSecondaryDetail(const UAssetEntry* Entry)
{
	InventoryDetail->InitializeEntryDetail(Entry);

	const UInventoryEntry* InventoryEntry = Cast<UInventoryEntry>(Entry);
	if (!IsValid(InventoryEntry))
	{
		return;
	}

	const FInventoryInstance* InventoryInstance = InventoryEntry->Item;
	if (!InventoryInstance)
	{
		return;
	}

	ActiveItemId = InventoryInstance->ItemId;
	AscensionInstance = InventoryInstance->Ascension;
	ToggleAscension(InventoryInstance);
}


void UInventoryAscensionDashboardUI::NativeConstruct()
{
	FGameUIDelegate::OnUIActionStarted.AddUObject(this, &UInventoryAscensionDashboardUI::LockControls);
	FGameUIDelegate::OnUIActionCompleted.AddUObject(this, &UInventoryAscensionDashboardUI::UnlockControls);

	RankUpButton->OnClicked.AddDynamic(this, &UInventoryAscensionDashboardUI::HandleRankUp);
	LevelUpButton->OnClicked.AddDynamic(this, &UInventoryAscensionDashboardUI::HandleLevelUp);

	AscensionSubsystem = UInventoryAscensionSubsystem::Get(GetGameInstance());

	Super::NativeConstruct();
}

void UInventoryAscensionDashboardUI::NativeDestruct()
{
	FGameUIDelegate::OnUIActionStarted.RemoveAll(this);
	FGameUIDelegate::OnUIActionCompleted.RemoveAll(this);

	RankUpButton->OnClicked.RemoveAll(this);
	LevelUpButton->OnClicked.RemoveAll(this);

	if (IsValid(StorageManager))
	{
		StorageManager->OnStorageUpdated.RemoveAll(this);
	}
	StorageManager = nullptr;

	AscensionSubsystem = nullptr;
	AscensionProvider = nullptr;

	Super::NativeDestruct();
}

