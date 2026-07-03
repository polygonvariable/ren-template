// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/QuestStorageManager.h"

// Project Headers
#include "Data/QuestStorage.h"


void UQuestStorageManager::AddActiveQuest(const FPrimaryAssetId& QuestId, const FGuid& Guid)
{
	if (LocalStorage)
	{
		TMap<FPrimaryAssetId, FGuid>& ActiveQuests = LocalStorage->ActiveQuests;
		if (!ActiveQuests.Contains(QuestId))
		{
			ActiveQuests.Add(QuestId, Guid);
		}
	}
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

const TMap<FPrimaryAssetId, FGuid>* UQuestStorageManager::GetActiveQuests() const
{
	if (!LocalStorage)
	{
		return nullptr;
	}
	return &LocalStorage->ActiveQuests;
}

UObject* UQuestStorageManager::GetStorage() const
{
	return LocalStorage;
}

void UQuestStorageManager::SetStorage(UObject* Storage)
{
	LocalStorage = Cast<UQuestStorage>(Storage);
}

FGameEventDelegate& UQuestStorageManager::GetOnStorageUpdated()
{
	return OnStorageUpdated;
}

