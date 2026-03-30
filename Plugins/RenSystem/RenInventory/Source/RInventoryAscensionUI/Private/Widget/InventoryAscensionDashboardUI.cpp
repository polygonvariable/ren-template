// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryAscensionDashboardUI.h"

// Engine Headers
#include "Components/Button.h"
#include "InstancedStruct.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetFilterProperty.h"
#include "Definition/Runtime/InventoryInstance.h"
#include "Filter/Criterion/FilterCriterion_Leaf.h"
#include "Interface/AscensionProvider.h"
#include "Library/AscensionLibrary.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetCollection.h"
#include "Storage/InventoryStorage.h"
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

	UInventoryStorage* InventoryStorage = InventorySubsystem->GetInventory(PrimarySourceId);
	if (IsValid(InventoryStorage) && bAutoRefresh)
	{
		InventoryStorage->OnStorageUpdated.AddUObject(this, &UInventoryAscensionDashboardUI::HandleOnItemUpdated);
	}

	Inventory = TWeakObjectPtr<UInventoryStorage>(InventoryStorage);
}

void UInventoryAscensionDashboardUI::RefreshDetail()
{
	UInventoryStorage* InventoryStorage = Inventory.Get();
	if (!IsValid(InventoryStorage))
	{
		return;
	}

	const FInventoryInstance* Item = InventoryStorage->GetInstanceById(GetActiveAssetId(), ActiveItemId);
	ToggleAscension(Item);
}



void UInventoryAscensionDashboardUI::EnableControls()
{
	LevelUpButton->SetIsEnabled(true);
	RankUpButton->SetIsEnabled(true);
}

void UInventoryAscensionDashboardUI::DisableControls()
{
	LevelUpButton->SetIsEnabled(false);
	RankUpButton->SetIsEnabled(false);
}



void UInventoryAscensionDashboardUI::ToggleAscension(const FInventoryInstance* Item)
{
	if (!ActiveAscensionProvider || !Item)
	{
		return;
	}

	int LevelPerRank = ActiveAscensionProvider->GetLevelInterval(Item->Ascension.Rank);
	int MaxLevel = ActiveAscensionProvider->GetMaxLevel();
	int MaxRank = ActiveAscensionProvider->GetMaxRank();

	if (UAscensionLibrary::IsRankUpRequired(Item->Ascension, LevelPerRank, MaxLevel, MaxRank))
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

	if (!ActiveAscensionProvider || !Item)
	{
		return;
	}

	RankItemCollection->ClearSubDetails();

	UFilterCriterion_Asset* AssetFilter = RankItemCollection->GetCriterionByName<UFilterCriterion_Asset>(FAssetFilterProperty::AssetId);
	if (IsValid(AssetFilter))
	{
		AssetFilter->Included.Empty();

		const UAssetCollection* ItemCollection = ActiveAscensionProvider->GetRankAssets(Item->Ascension);
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



void UInventoryAscensionDashboardUI::HandleTaskCallback(const FTaskResult& Result)
{
	if (Result.State == ETaskState::Pending)
	{
		DisableControls();
		LOG_WARNING(LogInventoryAscension, TEXT("Task Started"));
	}
	else
	{
		EnableControls();
		LOG_WARNING(LogInventoryAscension, TEXT("Task Finished, Message: %s"), *Result.Message);
	}
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
	
	AscensionSubsystem->AddExperiencePoints(PrimarySourceId, GetActiveAssetId(), ActiveItemId, MaterialAssetId, MaterialId, FTaskCallback::CreateUObject(this, &UInventoryAscensionDashboardUI::HandleTaskCallback));
}

void UInventoryAscensionDashboardUI::HandleRankUp()
{
	if (!IsValid(AscensionSubsystem))
	{
		LOG_ERROR(LogInventoryAscension, TEXT("AscensionSubsystem is not valid"));
		return;
	}

	AscensionSubsystem->AddRankPoints(PrimarySourceId, GetActiveAssetId(), ActiveItemId, FTaskCallback::CreateUObject(this, &UInventoryAscensionDashboardUI::HandleTaskCallback));
}

void UInventoryAscensionDashboardUI::HandleOnItemUpdated()
{
	RefreshDetail();
}



void UInventoryAscensionDashboardUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	InventoryDetail->InitializeAssetDetail(Asset);

	const IAscensionProvider* AscensionProvider = Cast<IAscensionProvider>(Asset);
	if (AscensionProvider)
	{
		ActiveAscensionProvider = AscensionProvider;

		UFilterCriterion_Asset* AssetIdFilter = LevelItemCollection->GetCriterionByName<UFilterCriterion_Asset>(FAssetFilterProperty::AssetId);
		if (IsValid(AssetIdFilter))
		{
			AssetIdFilter->Included.Empty();

			const UAssetCollection* ExperienceCollection = AscensionProvider->GetExperienceAssets(AscensionData);
			if (IsValid(ExperienceCollection))
			{
				TArray<FPrimaryAssetId> AssetList;
				ExperienceCollection->GetAssetIds(AssetList);

				AssetIdFilter->Included.Append(AssetList);
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

	AscensionData = InventoryInstance->Ascension;
	ToggleAscension(InventoryInstance);
}



void UInventoryAscensionDashboardUI::NativeConstruct()
{
	RankUpButton->OnClicked.AddDynamic(this, &UInventoryAscensionDashboardUI::HandleRankUp);
	LevelUpButton->OnClicked.AddDynamic(this, &UInventoryAscensionDashboardUI::HandleLevelUp);

	AscensionSubsystem = UInventoryAscensionSubsystem::Get(GetGameInstance());

	Super::NativeConstruct();
}

void UInventoryAscensionDashboardUI::NativeDestruct()
{
	RankUpButton->OnClicked.RemoveAll(this);
	LevelUpButton->OnClicked.RemoveAll(this);

	UInventoryStorage* InventoryStorage = Inventory.Get();
	if (IsValid(InventoryStorage))
	{
		InventoryStorage->OnStorageUpdated.RemoveAll(this);
	}
	Inventory.Reset();

	AscensionSubsystem = nullptr;
	ActiveAscensionProvider = nullptr;

	Super::NativeDestruct();
}

