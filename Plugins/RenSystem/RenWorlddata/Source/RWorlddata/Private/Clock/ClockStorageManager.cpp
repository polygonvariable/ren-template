// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Clock/ClockStorageManager.h"

// Project Headers
#include "Clock/ClockStorage.h"


FClockInstance UClockStorageManager::GetClockInstance(FName WorldId)
{
	if (!IsValid(LocalStorage))
	{
		return FClockInstance();
	}
	return LocalStorage->Clocks.FindOrAdd(WorldId);
}

void UClockStorageManager::SetClockInstance(FName WorldId, FClockInstance ClockInstance)
{
	if (IsValid(LocalStorage))
	{
		LocalStorage->Clocks.Add(WorldId, ClockInstance);
	}
}

UObject* UClockStorageManager::GetStorage() const
{
	return LocalStorage;
}

void UClockStorageManager::SetStorage(UObject* Storage)
{
	LocalStorage = Cast<UClockStorage>(Storage);
}

