// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarAscensionDashboardUI.h"

// Engine Headers
#include "Components/Button.h"
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Core/AscensionLibrary.h"
#include "Core/Type/AssetDetail.h"
#include "Core/Type/AssetFilterProperty.h"
#include "Core/Type/Runtime/AvatarInstance.h"
#include "Data/AscensionFragment.h"
#include "Data/AssetCollection.h"
#include "Data/CoreDataAsset.h"
#include "Delegate/GameUIDelegate.h"
#include "Filter/Criterion/FilterCriterion_Leaf.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/AvatarAscensionSubsystem.h"
#include "System/AvatarStorageManager.h"
#include "System/AvatarSubsystem.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/AssetDetailUI.h"
#include "Widget/AssetEntry.h"
#include "Widget/AvatarEntry.h"


void UAvatarAscensionDashboardUI::InitializeDetail()
{
	AvatarDetail->PrimarySourceId = PrimarySourceId;
	AvatarDetail->InitializeDetail();

	LevelItemCollection->InitializeCollection();
	RankItemCollection->InitializeCollection();

	UAvatarSubsystem* AvatarSubsystem = UAvatarSubsystem::Get(GetGameInstance());
	if (!IsValid(AvatarSubsystem))
	{
		return;
	}

	StorageManager = AvatarSubsystem->GetStorageManager();
	if (IsValid(StorageManager) && bAutoRefresh)
	{
		StorageManager->OnStorageUpdated.AddUObject(this, &UAvatarAscensionDashboardUI::RefreshDetail);
	}
}

void UAvatarAscensionDashboardUI::RefreshDetail()
{
	if (!IsValid(StorageManager))
	{
		return;
	}

	const FAvatarInstance* Instance = StorageManager->GetInstance(GetActiveAssetId());
	ToggleAscension(Instance);
}


void UAvatarAscensionDashboardUI::ToggleAscension(const FAvatarInstance* Instance)
{
	if (!IsValid(AscensionFragment) || !Instance)
	{
		return;
	}

	int LevelInterval = AscensionFragment->GetLevelInterval(Instance->Ascension.Rank);
	int MaxLevel = AscensionFragment->GetMaxLevel();
	int MaxRank = AscensionFragment->GetMaxRank();

	if (FAscensionLibrary::IsRankUpRequired(Instance->Ascension, LevelInterval, MaxLevel, MaxRank))
	{
		ToggleRankUp(Instance);
	}
	else
	{
		ToggleLevelUp(Instance);
	}
}

void UAvatarAscensionDashboardUI::ToggleLevelUp(const FAvatarInstance* Instance)
{
	LevelUpButton->SetVisibility(ESlateVisibility::Visible);
	RankUpButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UAvatarAscensionDashboardUI::ToggleRankUp(const FAvatarInstance* Instance)
{
	LevelUpButton->SetVisibility(ESlateVisibility::Collapsed);
	RankUpButton->SetVisibility(ESlateVisibility::Visible);

	if (!IsValid(AscensionFragment) || !Instance)
	{
		return;
	}

	RankItemCollection->ClearSubDetails();

	UFilterCriterion_Asset* AssetFilter = RankItemCollection->GetCriterionByName<UFilterCriterion_Asset>(FAssetFilterProperty::AssetId);
	if (IsValid(AssetFilter))
	{
		AssetFilter->Included.Empty();

		const UAssetCollection* ItemCollection = AscensionFragment->GetRankAssets(Instance->Ascension);
		if (IsValid(ItemCollection))
		{
			TMap<FPrimaryAssetId, FAssetDetail> AssetList;
			ItemCollection->GetAssetList(AssetList);

			for (const TPair<FPrimaryAssetId, FAssetDetail>& Kv : AssetList)
			{
				const FPrimaryAssetId& AssetId = Kv.Key;

				AssetFilter->Included.Add(AssetId);
				RankItemCollection->AddSubDetails(AssetId, FInstancedStruct::Make(Kv.Value));
			}
		}
	}

	RankItemCollection->RefreshEntries();
}


void UAvatarAscensionDashboardUI::HandleLevelUp()
{
	const UAssetEntry* Entry = LevelItemCollection->GetSelectedEntry();
	if (!IsValid(AscensionSubsystem) || !IsValid(Entry))
	{
		LOG_ERROR(LogAvatarAscension, TEXT("AscensionSubsystem, Entry is not valid"));
		return;
	}

	FGuid MaterialId = Entry->GetAssetInstanceId();
	FPrimaryAssetId MaterialAssetId = Entry->AssetId;
	
	AscensionSubsystem->TryAddExperiencePoints(PrimarySourceId, GetActiveAssetId(), MaterialAssetId, MaterialId);
}

void UAvatarAscensionDashboardUI::HandleRankUp()
{
	if (!IsValid(AscensionSubsystem))
	{
		LOG_ERROR(LogAvatarAscension, TEXT("AscensionSubsystem is not valid"));
		return;
	}

	AscensionSubsystem->TryAddRankPoints(PrimarySourceId, GetActiveAssetId());
}


TArray<UWidget*> UAvatarAscensionDashboardUI::GetLockingControls_Implementation() const
{
	TArray<UWidget*> Widgets;

	Widgets.Add(LevelUpButton);
	Widgets.Add(RankUpButton);

	return Widgets;
}

void UAvatarAscensionDashboardUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	AvatarDetail->InitializeAssetDetail(Asset);

	AscensionFragment = Asset->FindFragmentByClass<UAscensionFragment>();
	if (IsValid(AscensionFragment))
	{
		UFilterCriterion_Asset* AssetFilter = LevelItemCollection->GetCriterionByName<UFilterCriterion_Asset>(FAssetFilterProperty::AssetId);
		if (IsValid(AssetFilter))
		{
			AssetFilter->Included.Empty();

			const UAssetCollection* ItemCollection = AscensionFragment->GetExperienceAssets(AscensionInstance);
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

void UAvatarAscensionDashboardUI::SetSecondaryDetail(const UAssetEntry* Entry)
{
	AvatarDetail->InitializeEntryDetail(Entry);

	const UAvatarEntry* AvatarEntry = Cast<UAvatarEntry>(Entry);
	if (!IsValid(AvatarEntry))
	{
		return;
	}

	const FAvatarInstance* AvatarInstance = AvatarEntry->AvatarInstance;
	if (!AvatarInstance)
	{
		return;
	}

	AscensionInstance = AvatarInstance->Ascension;
	ToggleAscension(AvatarInstance);
}


void UAvatarAscensionDashboardUI::NativeConstruct()
{
	FGameUIDelegate::OnUIActionStarted.AddUObject(this, &UAvatarAscensionDashboardUI::LockControls);
	FGameUIDelegate::OnUIActionCompleted.AddUObject(this, &UAvatarAscensionDashboardUI::UnlockControls);

	RankUpButton->OnClicked.AddDynamic(this, &UAvatarAscensionDashboardUI::HandleRankUp);
	LevelUpButton->OnClicked.AddDynamic(this, &UAvatarAscensionDashboardUI::HandleLevelUp);

	AscensionSubsystem = UAvatarAscensionSubsystem::Get(GetGameInstance());

	Super::NativeConstruct();
}

void UAvatarAscensionDashboardUI::NativeDestruct()
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
	AscensionFragment = nullptr;

	Super::NativeDestruct();
}

