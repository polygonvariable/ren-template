// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/InventorySettings.h"

// Project Headers
#include "Data/InventoryStorage.h"
#include "System/InventoryStorageManager.h"
#include "System/InventorySubsystem.h"


UInventorySettings::UInventorySettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	StorageId = TEXT("inventory001");
	StorageClass = UInventoryStorage::StaticClass();
	StorageManagerClass = UInventoryStorageManager::StaticClass();

	SubsystemClass = UInventorySubsystem::StaticClass();
}

const UInventorySettings* UInventorySettings::Get()
{
	return GetDefault<UInventorySettings>();
}

