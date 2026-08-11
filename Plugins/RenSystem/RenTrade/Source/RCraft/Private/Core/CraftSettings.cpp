// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/CraftSettings.h"

// Project Header
#include "Auth/AACraftClaimItem.h"
#include "Auth/AACraftItem.h"
#include "Data/CraftStorage.h"
#include "System/CraftStorageManager.h"
#include "System/CraftSubsystem.h"


UCraftSettings::UCraftSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	StorageId = TEXT("craft001");
	StorageClass = UCraftStorage::StaticClass();
	StorageManagerClass = UCraftStorageManager::StaticClass();

	SubsystemClass = UCraftSubsystem::StaticClass();

	AACraftItemClass = UAACraftItem::StaticClass();
	AAClaimItemClass = UAACraftClaimItem::StaticClass();
}

const UCraftSettings* UCraftSettings::Get()
{
	return GetDefault<UCraftSettings>();
}

