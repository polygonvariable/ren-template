// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/AvatarStorageManager.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Core/AvatarSettings.h"
#include "Core/CharacterPrimaryAsset.h"
#include "Core/Type/AvatarQueryType.h"
#include "Core/Type/AvatarSortType.h"
#include "Core/Type/Runtime/AvatarInstance.h"
#include "Data/AvatarStorage.h"
#include "Core/Type/AssetFilterProperty.h"
#include "Definition/FilterContext.h"
#include "Filter/FilterCriterion.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


bool UAvatarStorageManager::AddInstance(const FPrimaryAssetId& AssetId, int Quantity)
{
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!IsValid(AssetManager))
	{
		LOG_ERROR(LogAvatar, TEXT("AssetManager is invalid"));
		return false;
	}

	if (!AddInstance_Internal(AssetManager, AssetId))
	{
		LOG_ERROR(LogAvatar, TEXT("Failed to add instance"));
		return false;
	}

	OnStorageUpdated.Broadcast();
	return true;
}

bool UAvatarStorageManager::RemoveInstance(const FPrimaryAssetId& AssetId, int Quantity)
{
	if (!RemoveInstance_Internal(AssetId))
	{
		return false;
	}

	OnStorageUpdated.Broadcast();
	return true;
}

bool UAvatarStorageManager::ContainInstance(const FPrimaryAssetId& AssetId, int Quantity) const
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}
	return LocalStorage->AvatarCollection.Contains(AssetId);
}

FGameEventDelegate& UAvatarStorageManager::GetOnAssetInstanceCollectionUpdated()
{
	return OnStorageUpdated;
}

bool UAvatarStorageManager::UpdateInstance(const FPrimaryAssetId& AssetId, TFunctionRef<void(FAvatarInstance*)> Callback)
{
	if (!IsValid(LocalStorage))
	{
		LOG_ERROR(LogAvatar, TEXT("AvatarStorage is invalid"));
		return false;
	}

	FAvatarInstance* Instance = LocalStorage->AvatarCollection.Find(AssetId);
	if (!Instance)
	{
		return false;
	}

	Callback(Instance);
	OnStorageUpdated.Broadcast();
	return true;
}


const FAscensionData* UAvatarStorageManager::GetAscensionInstance(const FPrimaryAssetId& AssetId, const FGuid& InstanceId) const
{
	const FAvatarInstance* Instance = GetInstanceById(InstanceId);
	if (!Instance)
	{
		return nullptr;
	}
	return &Instance->Ascension;
}


const FAvatarInstance* UAvatarStorageManager::GetInstance(const FPrimaryAssetId& AssetId) const
{
	if (!IsValid(LocalStorage))
	{
		return nullptr;
	}
	return LocalStorage->AvatarCollection.Find(AssetId);
}

const FAvatarInstance* UAvatarStorageManager::GetInstanceById(const FGuid& InstanceId) const
{
	if (!IsValid(LocalStorage))
	{
		return nullptr;
	}

	const TMap<FPrimaryAssetId, FAvatarInstance>& AvatarCollection = LocalStorage->AvatarCollection;
	for (const TPair<FPrimaryAssetId, FAvatarInstance>& Kv : AvatarCollection)
	{
		if (Kv.Value.AvatarId == InstanceId)
		{
			return &Kv.Value;
		}
	}

	return nullptr;
}


void UAvatarStorageManager::QueryInstances(const UFilterCriterion* FilterCriterion, const FAvatarQueryRule& QueryRule, TArray<FAvatarSortEntry>& OutSortedItems)
{
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!IsValid(AssetManager))
	{
		return;
	}
	
	if (QueryRule.QuerySource == EAssetQuerySource::Asset)
	{
		QueryAssetItems(AssetManager, FilterCriterion, OutSortedItems);
	}
	else
	{
		QueryInstanceItems(AssetManager, FilterCriterion, OutSortedItems);
	}

	HandleItemSorting(OutSortedItems, QueryRule);
}

void UAvatarStorageManager::HandleItemSorting(TArray<FAvatarSortEntry>& SortedItems, const FAvatarQueryRule& QueryRule) const
{
	const ESortDirection& SortDirection = QueryRule.SortDirection;
	const EAvatarSortType& SortType = QueryRule.SortType;

	switch (SortType)
	{
	case EAvatarSortType::None:
		LOG_WARNING(LogAvatar, TEXT("No sort implemented"));
		break;
	case EAvatarSortType::Alphabetical:
		SortedItems.Sort([SortDirection](const FAvatarSortEntry& A, const FAvatarSortEntry& B)
			{
				return (SortDirection == ESortDirection::Ascending) ?
					A.ItemName.ToString() < B.ItemName.ToString() :
					A.ItemName.ToString() > B.ItemName.ToString();
			}
		);
		break;
	case EAvatarSortType::Level:
		SortedItems.Sort([SortDirection](const FAvatarSortEntry& A, const FAvatarSortEntry& B)
			{
				if (!A.Instance || !B.Instance) return false;
				return (SortDirection == ESortDirection::Ascending) ?
					A.Instance->Ascension.Level < B.Instance->Ascension.Level :
					A.Instance->Ascension.Level > B.Instance->Ascension.Level;
			}
		);
		break;
	case EAvatarSortType::Rank:
		SortedItems.Sort([SortDirection](const FAvatarSortEntry& A, const FAvatarSortEntry& B)
			{
				if (!A.Instance || !B.Instance) return false;
				return (SortDirection == ESortDirection::Ascending) ?
					A.Instance->Ascension.Rank < B.Instance->Ascension.Rank :
					A.Instance->Ascension.Rank > B.Instance->Ascension.Rank;
			}
		);
		break;
	default:
		LOG_WARNING(LogAvatar, TEXT("No sort implemented"));
		break;
	}
}

void UAvatarStorageManager::QueryAssetItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FAvatarSortEntry>& OutSortedItems) const
{
	TArray<FPrimaryAssetId> AssetIds;
	if (!AssetManager->GetPrimaryAssetIdList(FCharacterPrimaryAsset::GetAssetType(), AssetIds))
	{
		return;
	}

	for (const FPrimaryAssetId& AssetId : AssetIds)
	{
		FAssetData AssetData;
		if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
		{
			continue;
		}

		FText DisplayName = FText::GetEmpty();
		if (!FCharacterPrimaryAsset::GetDisplayName(AssetData, DisplayName))
		{
			continue;
		}

		if (IsValid(FilterCriterion))
		{
			FFilterContext Context;
			Context.SetValue(FAssetFilterProperty::AssetId, AssetId);

			if (!FilterCriterion->Evaluate(Context))
			{
				continue;
			}
		}

		OutSortedItems.Emplace(AssetId, DisplayName);
	}
}

void UAvatarStorageManager::QueryInstanceItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FAvatarSortEntry>& OutSortedItems) const
{
	if (!IsValid(LocalStorage))
	{
		return;
	}

	const TMap<FPrimaryAssetId, FAvatarInstance>& AvatarCollection = LocalStorage->AvatarCollection;
	for (const TPair<FPrimaryAssetId, FAvatarInstance>& Kv : AvatarCollection)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;

		FAssetData AssetData;
		if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
		{
			continue;
		}

		FText DisplayName = FText::GetEmpty();
		if (!FCharacterPrimaryAsset::GetDisplayName(AssetData, DisplayName))
		{
			continue;
		}

		if (IsValid(FilterCriterion))
		{
			FFilterContext Context;
			Context.SetValue(FAssetFilterProperty::AssetId, AssetId);

			if (!FilterCriterion->Evaluate(Context))
			{
				continue;
			}
		}

		OutSortedItems.Emplace(AssetId, DisplayName, &Kv.Value);
	}
}


bool UAvatarStorageManager::AddInstance_Internal(UAssetManager* AssetManager, const FPrimaryAssetId& AssetId)
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}

	TMap<FPrimaryAssetId, FAvatarInstance>& AvatarCollection = LocalStorage->AvatarCollection;
	if (!FCharacterPrimaryAsset::IsValid(AssetId) || AvatarCollection.Contains(AssetId))
	{
		return false;
	}

	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
	{
		return false;
	}

	int Health = 0;
	if (!FCharacterPrimaryAsset::GetHealth(AssetData, Health))
	{
		return false;
	}

	FGuid InstanceId = FGuid::NewGuid();
	AvatarCollection.Add(AssetId, FAvatarInstance(InstanceId, Health));
	return true;
}

bool UAvatarStorageManager::RemoveInstance_Internal(const FPrimaryAssetId& AssetId)
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}
	return LocalStorage->AvatarCollection.Remove(AssetId) > 0;
}


UObject* UAvatarStorageManager::GetStorage() const
{
	return LocalStorage;
}

void UAvatarStorageManager::SetStorage(UObject* Storage)
{
	LocalStorage = Cast<UAvatarStorage>(Storage);
}

void UAvatarStorageManager::OnStorageLoaded(bool bIsNew)
{
	if (IsValid(LocalStorage) && bIsNew)
	{
		LocalStorage->AvatarCollection = UAvatarSettings::Get()->DefaultAvatars;
	}
}

FGameEventDelegate& UAvatarStorageManager::GetOnStorageUpdated()
{
	return OnStorageUpdated;
}

