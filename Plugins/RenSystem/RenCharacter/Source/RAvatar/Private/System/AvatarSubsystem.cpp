// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/AvatarSubsystem.h"

// Project Headers
#include "Core/AvatarSettings.h"
#include "Data/AvatarAsset.h"
#include "Data/AvatarStorage.h"
#include "Delegate/GameLifecycleDelegate.h"
#include "Interface/StorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/AvatarStorageManager.h"


UAvatarStorageManager* UAvatarSubsystem::GetStorageManager() const
{
	if (!StorageProvider)
	{
		return nullptr;
	}

	FName StorageId = UAvatarSettings::Get()->StorageId;
	return StorageProvider->GetStorageManager<UAvatarStorageManager>(StorageId);
}

IAssetInstanceCollection* UAvatarSubsystem::GetInstanceCollection(const FName& CollectionId) const
{
	return Cast<IAssetInstanceCollection>(GetStorageManager());
}

FPrimaryAssetType UAvatarSubsystem::GetSupportedAssetType() const
{
	return UAvatarAsset::GetPrimaryAssetType();
}

FName UAvatarSubsystem::GetPrimaryCollectionId() const
{
	return UAvatarSettings::Get()->StorageId;
}

void UAvatarSubsystem::HandleOnPreGameInitialized()
{
	StorageProvider = IStorageProvider::Get(GetGameInstance());
	if (StorageProvider)
	{
		const UAvatarSettings* Settings = UAvatarSettings::Get();

		FStorageDefinition Definition;
		Definition.StorageId = Settings->StorageId;
		Definition.StorageClass = Settings->StorageClass;
		Definition.ManagerClass = Settings->StorageManagerClass;

		StorageProvider->LoadStorage(Definition, FTaskCallback());
	}
}

bool UAvatarSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return GetClass() == UAvatarSettings::Get()->SubsystemClass;
}

void UAvatarSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogAvatar, TEXT("AvatarSubsystem initialized"));

	FGameLifecycleDelegate::OnPreGameInitialized.AddUObject(this, &UAvatarSubsystem::HandleOnPreGameInitialized);
}

void UAvatarSubsystem::Deinitialize()
{
	FGameLifecycleDelegate::OnPreGameInitialized.RemoveAll(this);
	StorageProvider = nullptr;

	LOG_WARNING(LogAvatar, TEXT("AvatarSubsystem deinitialized"));
	Super::Deinitialize();
}

UAvatarSubsystem* UAvatarSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UAvatarSubsystem* UAvatarSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UAvatarSubsystem>();
}

