// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Settings/ShopSettings.h"

// Project Header
#include "Storage/ShopStorage.h"
#include "Storage/ShopStorageManager.h"
#include "Subsystem/ShopSubsystem.h"


UShopSettings::UShopSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	StorageId = TEXT("shop001");
	StorageClass = UShopStorage::StaticClass();
	StorageManagerClass = UShopStorageManager::StaticClass();
	SubsystemClass = UShopSubsystem::StaticClass();
}

const UShopSettings* UShopSettings::Get()
{
	return GetDefault<UShopSettings>();
}

