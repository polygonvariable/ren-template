// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/EnemySettings.h"

// Project Header
#include "Data/EnemyStorage.h"
#include "System/EnemyStorageManager.h"
#include "System/EnemySubsystem.h"


UEnemySettings::UEnemySettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	StorageId = TEXT("enemy001");
	StorageClass = UEnemyStorage::StaticClass();
	StorageManagerClass = UEnemyStorageManager::StaticClass();

	SubsystemClass = UEnemySubsystem::StaticClass();
}

const UEnemySettings* UEnemySettings::Get()
{
	return GetDefault<UEnemySettings>();
}

