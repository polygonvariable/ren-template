// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "StorageSubsystem.h"

// Engine Headers
#include "Kismet/GameplayStatics.h"

// Project Headers
#include "RenGlobal/Public/Macro/LogMacro.h"
#include "Storage.h"



void UStorageSubsystem::ReadStorage(FName SlotId)
{
	if (DoesStorageExist(SlotId))
	{
		Storage = Cast<UStorage>(UGameplayStatics::LoadGameFromSlot(SlotId.ToString(), 0));
		LOG_INFO(LogTemp, TEXT("Storage loaded from slot"));
	}
	else
	{
		USaveGame* NewStorage = UGameplayStatics::CreateSaveGameObject(UStorage::StaticClass());
		if (!IsValid(NewStorage))
		{
			LOG_ERROR(LogTemp, TEXT("Failed to create save game object"));
			return;
		}

		if (!UGameplayStatics::SaveGameToSlot(NewStorage, SlotId.ToString(), 0))
		{
			LOG_ERROR(LogTemp, TEXT("Failed to save storage to slot"));
			return;
		}
		Storage = Cast<UStorage>(NewStorage);

		LOG_INFO(LogTemp, TEXT("Storage created and saved to slot"));
	}
}

void UStorageSubsystem::UpdateStorage(FName SlotId)
{
	if (!IsValid(Storage))
	{
		LOG_ERROR(LogTemp, TEXT("Storage is not valid"));
		return;
	}

	if (!UGameplayStatics::SaveGameToSlot(Storage, SlotId.ToString(), 0))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to save storage to slot"));
		return;
	}

	LOG_INFO(LogTemp, TEXT("Storage updated and saved to slot"));
}

bool UStorageSubsystem::DoesStorageExist(FName SlotId)
{
	return UGameplayStatics::DoesSaveGameExist(SlotId.ToString(), 0);
}

UStorage* UStorageSubsystem::GetLocalStorage()
{
	// WHAT !?!
	return IsValid(Storage) ? Storage : nullptr;
}



USaveGame* UStorageSubsystem::IGetLocalStorage()
{
	return Storage;
}



bool UStorageSubsystem::ShouldCreateSubsystem(UObject* Object) const
{
	return true;
}

void UStorageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ReadStorage();
}

void UStorageSubsystem::Deinitialize()
{
	UpdateStorage();
	Super::Deinitialize();
}

