// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Storage/RewardStorageManager.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Storage/RewardStorage.h"


bool URewardStorageManager::AddInstance(const FPrimaryAssetId& AssetId, int Quantity)
{
	return false;
}

bool URewardStorageManager::RemoveInstance(const FPrimaryAssetId& AssetId, int Quantity)
{
	return false;
}

bool URewardStorageManager::ContainInstance(const FPrimaryAssetId& AssetId, int Quantity) const
{
	return false;
}

FGameEventDelegate& URewardStorageManager::GetOnAssetInstanceCollectionUpdated()
{
	return GetOnStorageUpdated();
}

UObject* URewardStorageManager::GetStorage() const
{
	return LocalStorage;
}

void URewardStorageManager::SetStorage(UObject* InStorage)
{
	LocalStorage = Cast<URewardStorage>(InStorage);
}

FGameEventDelegate& URewardStorageManager::GetOnStorageUpdated()
{
	return OnStorageUpdated;
}

