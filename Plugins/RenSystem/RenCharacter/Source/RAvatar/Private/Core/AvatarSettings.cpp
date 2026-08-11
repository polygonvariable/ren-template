// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/AvatarSettings.h"

// Project Header
#include "Data/AvatarStorage.h"
#include "System/AvatarStorageManager.h"
#include "System/AvatarSubsystem.h"


UAvatarSettings::UAvatarSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	StorageId = TEXT("avatar001");
	StorageClass = UAvatarStorage::StaticClass();
	StorageManagerClass = UAvatarStorageManager::StaticClass();

	SubsystemClass = UAvatarSubsystem::StaticClass();
}

const UAvatarSettings* UAvatarSettings::Get()
{
	return GetDefault<UAvatarSettings>();
}

