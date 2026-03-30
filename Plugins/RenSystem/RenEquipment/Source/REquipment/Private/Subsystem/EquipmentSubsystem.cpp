// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/EquipmentSubsystem.h"

// Engine Headers

// Project Headers
#include "Delegate/GameLifecycleDelegates.h"
#include "Interface/IStorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/EquipmentSettings.h"
#include "Storage/EquipmentStorage.h"


void UEquipmentSubsystem::SyncEquipment(const FGuid& OwnerId) const
{
	return OnSyncEquipment.Broadcast(OwnerId);
}

UEquipmentStorage* UEquipmentSubsystem::GetEquipment() const
{
	IStorageProvider* StorageInterface = StorageProvider.Get();
	if (!StorageInterface)
	{
		return nullptr;
	}

	FName StorageId = UEquipmentSettings::Get()->StorageId;
	UStorage* Storage = StorageInterface->GetStorage(StorageId);

	return Cast<UEquipmentStorage>(Storage);
}

void UEquipmentSubsystem::OnPreGameInitialized()
{
	IStorageProvider* StorageInterface = IStorageProvider::Get(GetGameInstance());
	if (StorageInterface)
	{
		StorageInterface->LoadStorageFromSettings(UEquipmentSettings::Get());
		StorageProvider = TWeakInterfacePtr<IStorageProvider>(StorageInterface);
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

	FGameLifecycleDelegates::OnPreGameInitialized.AddUObject(this, &UEquipmentSubsystem::OnPreGameInitialized);
}

void UEquipmentSubsystem::Deinitialize()
{
	FGameLifecycleDelegates::OnPreGameInitialized.RemoveAll(this);
	StorageProvider.Reset();

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

