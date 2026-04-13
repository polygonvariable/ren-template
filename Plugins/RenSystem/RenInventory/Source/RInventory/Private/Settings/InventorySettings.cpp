// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Settings/InventorySettings.h"

// Project Headers
#include "Storage/InventoryStorage.h"


UInventorySettings::UInventorySettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");
}

const FName& UInventorySettings::GetStorageId() const
{
	return StorageId;
}

TSubclassOf<UStorage> UInventorySettings::GetStorageClass() const
{
	return StorageClass;
}

const UInventorySettings* UInventorySettings::Get()
{
	return GetDefault<UInventorySettings>();
}

