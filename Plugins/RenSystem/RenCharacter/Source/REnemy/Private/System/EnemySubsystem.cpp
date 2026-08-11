// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/EnemySubsystem.h"

// Project Headers
#include "Delegate/GameLifecycleDelegate.h"
#include "Interface/StorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Core/EnemySettings.h"
#include "Data/EnemyStorage.h"
#include "System/EnemyStorageManager.h"
#include "Util/SubsystemUtil.h"


UEnemyStorageManager* UEnemySubsystem::GetStorageManager()
{
	if (!IsValid(StorageManager))
	{
		if (!StorageProvider)
		{
			return nullptr;
		}
		const UEnemySettings* Settings = UEnemySettings::Get();
		StorageManager = StorageProvider->GetStorageManager<UEnemyStorageManager>(Settings->StorageId);
	}
	return StorageManager;
}

void UEnemySubsystem::HandleOnPreGameInitialized()
{
	StorageProvider = SubsystemUtil::GetSubsystemInterface<IStorageProvider>(GetGameInstance());
	if (StorageProvider)
	{
		const UEnemySettings* Settings = UEnemySettings::Get();

		FStorageDefinition Definition;
		Definition.StorageId = Settings->StorageId;
		Definition.StorageClass = Settings->StorageClass;
		Definition.ManagerClass = Settings->StorageManagerClass;

		StorageProvider->LoadStorage(Definition, FTaskCallback());
	}
}

bool UEnemySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UEnemySettings* Settings = UEnemySettings::Get();
	return GetClass() == Settings->SubsystemClass;
}

void UEnemySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogCharacterParty, TEXT("EnemySubsystem initialized"));

	FGameLifecycleDelegate::OnPreGameInitialized.AddUObject(this, &UEnemySubsystem::HandleOnPreGameInitialized);
}

void UEnemySubsystem::Deinitialize()
{
	FGameLifecycleDelegate::OnPreGameInitialized.RemoveAll(this);
	StorageProvider = nullptr;
	StorageManager = nullptr;

	LOG_WARNING(LogCharacterParty, TEXT("EnemySubsystem deinitialized"));
	Super::Deinitialize();
}

UEnemySubsystem* UEnemySubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UEnemySubsystem* UEnemySubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UEnemySubsystem>();
}

