// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/EnemyStorageManager.h"

// Project Headers
#include "Data/EnemyStorage.h"


void UEnemyStorageManager::AddKillTimestamp(const FGuid& EnemyId)
{
	FDateTime KillTime = FDateTime::Now();
	LocalStorage->KillTimestamps.Add(EnemyId, KillTime);
}

void UEnemyStorageManager::RemoveKillTimestamp(const FGuid& EnemyId)
{
	LocalStorage->KillTimestamps.Remove(EnemyId);
}

FDateTime UEnemyStorageManager::GetKillTimestamp(const FGuid& EnemyId) const
{
	const FDateTime* KillTimestamp = LocalStorage->KillTimestamps.Find(EnemyId);
	if (!KillTimestamp)
	{
		return FDateTime();
	}
	return *KillTimestamp;
}

bool UEnemyStorageManager::ContainsKillTimestamp(const FGuid& EnemyId) const
{
	return LocalStorage->KillTimestamps.Contains(EnemyId);
}

UObject* UEnemyStorageManager::GetStorage() const
{
	return LocalStorage;
}

void UEnemyStorageManager::SetStorage(UObject* Storage)
{
	LocalStorage = Cast<UEnemyStorage>(Storage);
}

FGameEventDelegate& UEnemyStorageManager::GetOnStorageUpdated()
{
	return OnStorageUpdated;
}

