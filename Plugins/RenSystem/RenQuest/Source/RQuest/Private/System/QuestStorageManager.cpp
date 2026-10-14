// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/QuestStorageManager.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Core/QuestPropertyDefinition.h"
#include "Core/Type/AssetFilterProperty.h"
#include "Data/QuestStorage.h"
#include "FilterCriterion.h"


FGuid UQuestStorageManager::GetActiveQuest(const FPrimaryAssetId& QuestId)
{
	if (!IsValid(LocalStorage))
	{
		return FGuid();
	}
	return LocalStorage->ActiveQuests.FindRef(QuestId);
}

void UQuestStorageManager::FindAddActiveQuest(const FPrimaryAssetId& QuestId, const FGuid& Guid)
{
	if (!IsValid(LocalStorage))
	{
		return;
	}

	LocalStorage->ActiveQuests.Add(QuestId, Guid);
}

bool UQuestStorageManager::AddActiveQuest(const FPrimaryAssetId& QuestId, const FGuid& Guid)
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}

	TMap<FPrimaryAssetId, FGuid>& ActiveQuests = LocalStorage->ActiveQuests;
	if (ActiveQuests.Contains(QuestId))
	{
		return false;
	}

	ActiveQuests.Add(QuestId, Guid);
	return true;
}

bool UQuestStorageManager::CompleteActiveQuest(const FPrimaryAssetId& QuestId)
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}

	TMap<FPrimaryAssetId, FGuid>& ActiveQuests = LocalStorage->ActiveQuests;
	if (!ActiveQuests.Contains(QuestId))
	{
		return false;
	}

	ActiveQuests.Remove(QuestId);
	LocalStorage->CompletedQuests.Add(QuestId, FDateTime::UtcNow());

	return true;
}

void UQuestStorageManager::UpdateActiveQuest(const FPrimaryAssetId& QuestId, const FGuid& Guid)
{
	if (LocalStorage)
	{
		TMap<FPrimaryAssetId, FGuid>& ActiveQuests = LocalStorage->ActiveQuests;
		if (ActiveQuests.Contains(QuestId))
		{
			ActiveQuests.Add(QuestId, Guid);
		}
	}
}

void UQuestStorageManager::RemoveActiveQuest(const FPrimaryAssetId& QuestId)
{
	if (LocalStorage)
	{
		TMap<FPrimaryAssetId, FGuid>& ActiveQuests = LocalStorage->ActiveQuests;
		ActiveQuests.Remove(QuestId);
	}
}



bool UQuestStorageManager::ContainsActiveQuest(const FPrimaryAssetId& QuestId, bool& OutResult)
{
	if (!LocalStorage)
	{
		OutResult = false;
		return false;
	}

	OutResult = LocalStorage->ActiveQuests.Contains(QuestId);
	return true;
}

bool UQuestStorageManager::ContainsCompletedQuest(const FPrimaryAssetId& QuestId, bool& OutResult)
{
	if (!LocalStorage)
	{
		OutResult = false;
		return false;
	}

	OutResult = LocalStorage->CompletedQuests.Contains(QuestId);
	return true;
}


void UQuestStorageManager::QueryQuests(const FQuestQueryDefinition& Query, TArray<FQuestQueryResult>& OutResult)
{
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!IsValid(LocalStorage) || !IsValid(AssetManager))
	{
		return;
	}

	const TInstancedStruct<FFilterCriterion>& FilterCriterion = Query.FilterRule.CriterionRoot;
	const FFilterCriterion* Criterion = FilterCriterion.GetPtr();

	if (Query.bActive)
	{
		const TMap<FPrimaryAssetId, FGuid>& Quests = LocalStorage->ActiveQuests;
		for (const TPair<FPrimaryAssetId, FGuid>& Kv : Quests)
		{
			if (!EvaluateQueryQuests(AssetManager, Criterion, Kv.Key))
			{
				continue;
			}

			FQuestQueryResult Result;
			Result.bActive = true;
			Result.ActiveNodeId = Kv.Value;
			Result.QuestAssetId = Kv.Key;

			OutResult.Add(MoveTemp(Result));
		}
	}
	else
	{
		const TMap<FPrimaryAssetId, FDateTime>& Quests = LocalStorage->CompletedQuests;
		for (const TPair<FPrimaryAssetId, FDateTime>& Kv : Quests)
		{
			if (!EvaluateQueryQuests(AssetManager, Criterion, Kv.Key))
			{
				continue;
			}

			FQuestQueryResult Result;
			Result.bActive = false;
			Result.QuestAssetId = Kv.Key;

			OutResult.Add(MoveTemp(Result));
		}
	}
}

bool UQuestStorageManager::EvaluateQueryQuests(UAssetManager* AssetManager, const FFilterCriterion* Criterion, const FPrimaryAssetId& AssetId) const
{
	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
	{
		ensureMsgf(false, TEXT("Failed to get asset data"));
		return false;
	}

	FName QuestType;
	FQuestAssetData::GetQuestType(AssetData, QuestType);

	if (Criterion)
	{
		FFilterContext Context;
		Context.SetValue(FAssetFilterProperty::AssetId, AssetId);
		Context.SetValue(FQuestFilterProperty::QuestType, QuestType);

		return Criterion->Evaluate(Context);
	}

	return true;
}

UObject* UQuestStorageManager::GetStorage() const
{
	return LocalStorage;
}

void UQuestStorageManager::SetStorage(UObject* Storage)
{
	LocalStorage = Cast<UQuestStorage>(Storage);
}

