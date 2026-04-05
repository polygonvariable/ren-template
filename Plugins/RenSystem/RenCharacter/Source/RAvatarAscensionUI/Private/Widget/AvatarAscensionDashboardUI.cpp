// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarAscensionDashboardUI.h"

// Engine Headers
#include "Components/Button.h"
#include "InstancedStruct.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetFilterProperty.h"
#include "Definition/Runtime/AvatarInstance.h"
#include "Filter/Criterion/FilterCriterion_Leaf.h"
#include "Interface/AscensionProvider.h"
#include "Library/AscensionLibrary.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetCollection.h"
#include "Storage/AvatarStorage.h"
#include "Subsystem/AvatarAscensionSubsystem.h"
#include "Subsystem/AvatarSubsystem.h"
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

	UAvatarStorage* AvatarCollection = AvatarSubsystem->GetAvatarCollection();
	if (IsValid(AvatarCollection) && !bAutoRefresh)
	{
		AvatarCollection->OnStorageUpdated.AddUObject(this, &UAvatarAscensionDashboardUI::RefreshDetail);
	}

	AvatarStorage = TWeakObjectPtr<UAvatarStorage>(AvatarCollection);
}

void UAvatarAscensionDashboardUI::RefreshDetail()
{
	UAvatarStorage* AvatarCollection = AvatarStorage.Get();
	if (!IsValid(AvatarCollection))
	{
		return;
	}

	const FAvatarInstance* Instance = AvatarCollection->GetInstance(GetActiveAssetId());
	ToggleAscension(Instance);
}



void UAvatarAscensionDashboardUI::EnableControls()
{
	LevelUpButton->SetIsEnabled(true);
	RankUpButton->SetIsEnabled(true);
}

void UAvatarAscensionDashboardUI::DisableControls()
{
	LevelUpButton->SetIsEnabled(false);
	RankUpButton->SetIsEnabled(false);
}



void UAvatarAscensionDashboardUI::ToggleAscension(const FAvatarInstance* Instance)
{
	if (!ActiveAscensionProvider || !Instance)
	{
		return;
	}

	int LevelPerRank = ActiveAscensionProvider->GetLevelInterval(Instance->Ascension.Rank);
	int MaxLevel = ActiveAscensionProvider->GetMaxLevel();
	int MaxRank = ActiveAscensionProvider->GetMaxRank();

	if (FAscensionLibrary::IsRankUpRequired(Instance->Ascension, LevelPerRank, MaxLevel, MaxRank))
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

	if (!ActiveAscensionProvider || !Instance)
	{
		return;
	}

	RankItemCollection->ClearSubDetails();

	UFilterCriterion_Asset* AssetFilter = RankItemCollection->GetCriterionByName<UFilterCriterion_Asset>(FAssetFilterProperty::AssetId);
	if (IsValid(AssetFilter))
	{
		AssetFilter->Included.Empty();

		const UAssetCollection* ItemCollection = ActiveAscensionProvider->GetRankAssets(Instance->Ascension);
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



void UAvatarAscensionDashboardUI::HandleTaskCallback(const FTaskResult& Result)
{
	if (Result.State == ETaskState::Pending)
	{
		DisableControls();
		LOG_WARNING(LogAvatarAscension, TEXT("Task Started"));
	}
	else
	{
		EnableControls();
		LOG_WARNING(LogAvatarAscension, TEXT("Task Finished, Message: %s"), *Result.Message);
	}
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
	
	AscensionSubsystem->AddExperiencePoints(PrimarySourceId, GetActiveAssetId(), MaterialAssetId, MaterialId, FTaskCallback::CreateUObject(this, &UAvatarAscensionDashboardUI::HandleTaskCallback));
}

void UAvatarAscensionDashboardUI::HandleRankUp()
{
	if (!IsValid(AscensionSubsystem))
	{
		LOG_ERROR(LogAvatarAscension, TEXT("AscensionSubsystem is not valid"));
		return;
	}

	AscensionSubsystem->AddRankPoints(PrimarySourceId, GetActiveAssetId(), FTaskCallback::CreateUObject(this, &UAvatarAscensionDashboardUI::HandleTaskCallback));
}



void UAvatarAscensionDashboardUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	AvatarDetail->InitializeAssetDetail(Asset);

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

void UAvatarAscensionDashboardUI::SetSecondaryDetail(const UAssetEntry* Entry)
{
	AvatarDetail->InitializeEntryDetail(Entry);

	const UAvatarEntry* AvatarEntry = Cast<UAvatarEntry>(Entry);
	if (IsValid(AvatarEntry))
	{
		return;
	}

	const FAvatarInstance* AvatarInstance = AvatarEntry->AvatarInstance;
	if (!AvatarInstance)
	{
		return;
	}

	AscensionData = AvatarInstance->Ascension;
	ToggleAscension(AvatarInstance);
}



void UAvatarAscensionDashboardUI::NativeConstruct()
{
	RankUpButton->OnClicked.AddDynamic(this, &UAvatarAscensionDashboardUI::HandleRankUp);
	LevelUpButton->OnClicked.AddDynamic(this, &UAvatarAscensionDashboardUI::HandleLevelUp);

	AscensionSubsystem = UAvatarAscensionSubsystem::Get(GetGameInstance());

	Super::NativeConstruct();
}

void UAvatarAscensionDashboardUI::NativeDestruct()
{
	RankUpButton->OnClicked.RemoveAll(this);
	LevelUpButton->OnClicked.RemoveAll(this);

	UAvatarStorage* AvatarCollection = AvatarStorage.Get();
	if (IsValid(AvatarCollection))
	{
		AvatarCollection->OnStorageUpdated.RemoveAll(this);
	}
	AvatarStorage.Reset();

	AscensionSubsystem = nullptr;
	ActiveAscensionProvider = nullptr;

	Super::NativeDestruct();
}

