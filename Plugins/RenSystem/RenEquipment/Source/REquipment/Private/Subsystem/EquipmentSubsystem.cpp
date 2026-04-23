// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/EquipmentSubsystem.h"

// Project Headers
#include "Delegate/GameLifecycleDelegate.h"
#include "Interface/IStorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/EquipmentSettings.h"
#include "Storage/EquipmentStorage.h"


void UEquipmentSubsystem::SyncEquipment(const FGuid& OwnerInstanceId) const
{
	return OnSyncEquipment.Broadcast(OwnerInstanceId);
}

UEquipmentStorage* UEquipmentSubsystem::GetEquipmentStorage() const
{
	if (!StorageProvider)
	{
		return nullptr;
	}

	FName StorageId = UEquipmentSettings::Get()->StorageId;
	return StorageProvider->GetStorage<UEquipmentStorage>(StorageId);
}

void UEquipmentSubsystem::OnPreGameInitialized()
{
	StorageProvider = IStorageProvider::Get(GetGameInstance());
	if (StorageProvider)
	{
		StorageProvider->LoadStorageFromSettings(UEquipmentSettings::Get());
	}
}

bool UEquipmentSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return GetClass() == UEquipmentSettings::Get()->SubsystemClass;
}

void UEquipmentSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogEquipment, TEXT("EquipmentSubsystem initialized"));

	FGameLifecycleDelegate::OnPreGameInitialized.AddUObject(this, &UEquipmentSubsystem::OnPreGameInitialized);
}

void UEquipmentSubsystem::Deinitialize()
{
	FGameLifecycleDelegate::OnPreGameInitialized.RemoveAll(this);
	StorageProvider = nullptr;

	LOG_WARNING(LogEquipment, TEXT("EquipmentSubsystem deinitialized"));
	Super::Deinitialize();
}


UEquipmentSubsystem* UEquipmentSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UEquipmentSubsystem* UEquipmentSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UEquipmentSubsystem>();
}

