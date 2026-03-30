// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Settings/EquipmentSettings.h"

// Project Headers
#include "Storage/EquipmentStorage.h"


UEquipmentSettings::UEquipmentSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");
}

const FName& UEquipmentSettings::GetStorageId() const
{
	return StorageId;
}

TSubclassOf<UStorage> UEquipmentSettings::GetStorageClass() const
{
	return StorageClass;
}

const UEquipmentSettings* UEquipmentSettings::Get()
{
	return GetDefault<UEquipmentSettings>();
}

