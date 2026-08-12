// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/InventorySubsystem.h"

// Project Headers
#include "Asset/InventoryAsset.h"
#include "Delegate/GameLifecycleDelegate.h"
#include "Interface/StorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/InventorySettings.h"
#include "Storage/InventoryStorage.h"
#include "Storage/InventoryStorageManager.h"


UInventoryStorageManager* UInventorySubsystem::GetStorageManager(const FName& InventoryId) const
{
	if (!StorageProvider)
	{
		return nullptr;
	}
	return StorageProvider->GetStorageManager<UInventoryStorageManager>(InventoryId);
}

IAssetInstanceCollection* UInventorySubsystem::GetInstanceCollection(const FName& SourceId) const
{
	return Cast<IAssetInstanceCollection>(GetStorageManager(SourceId));
}

FPrimaryAssetType UInventorySubsystem::GetSupportedAssetType() const
{
	return UInventoryAsset::GetPrimaryAssetType();
}

FName UInventorySubsystem::GetPrimaryCollectionId() const
{
	return UInventorySettings::Get()->StorageId;
}

void UInventorySubsystem::HandleOnPreGameInitialized()
{
	StorageProvider = IStorageProvider::Get(GetGameInstance());
	if (StorageProvider)
	{
		const UInventorySettings* Settings = UInventorySettings::Get();

		FStorageDefinition StorageDefinition;
		StorageDefinition.StorageId = Settings->StorageId;
		StorageDefinition.StorageClass = Settings->StorageClass;
		StorageDefinition.ManagerClass = Settings->StorageManagerClass;

		StorageProvider->LoadStorage(StorageDefinition, FTaskCallback());
	}
}

bool UInventorySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return GetClass() == UInventorySettings::Get()->SubsystemClass;
}

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogInventory, TEXT("InventorySubsystem initialized"));

	FGameLifecycleDelegate::OnPreGameInitialized.AddUObject(this, &UInventorySubsystem::HandleOnPreGameInitialized);
}

void UInventorySubsystem::Deinitialize()
{
	FGameLifecycleDelegate::OnPreGameInitialized.RemoveAll(this);
	StorageProvider = nullptr;

	LOG_WARNING(LogInventory, TEXT("InventorySubsystem deinitialized"));
	Super::Deinitialize();
}

UInventorySubsystem* UInventorySubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UInventorySubsystem* UInventorySubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UInventorySubsystem>();
}

