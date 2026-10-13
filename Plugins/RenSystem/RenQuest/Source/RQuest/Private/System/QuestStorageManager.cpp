// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/QuestStorageManager.h"

// Project Headers
#include "Data/QuestStorage.h"



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

