// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "StorageSubsystem.h"

// Engine Headers
#include "Kismet/GameplayStatics.h"

// Project Headers
#include "RenCoreLibrary/Public/LogMacro.h"
#include "RenCore/Public/Delegate/LatentDelegates.h"

#include "RenStorage/Public/Storage.h"



bool UStorageSubsystem::ReadStorage(FName SlotId, int UserIndex)
{
	if (IsValid(CurrentStorage) && CurrentSlotId.IsValid())
	{
		UpdateStorage(CurrentSlotId, CurrentUserIndex);
	}

	FString SlotName = SlotId.ToString();

	if (DoesStorageExist(SlotId))
	{
		if (USaveGame* LoadedSaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex))
		{
			CurrentStorage = Cast<UStorage>(LoadedSaveGame);
			CurrentSlotId = SlotId;
			CurrentUserIndex = UserIndex;

			LOG_INFO(LogTemp, TEXT("Storage loaded from slot: %s"), *SlotName);
			return true;
		}

		LOG_ERROR(LogTemp, TEXT("Failed to load storage from slot: %s"), *SlotName);
		return false;
	}

	if (!CreateNewStorage(SlotId, UserIndex))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to create new storage for slot: %s"), *SlotName);
		return false;
	}

	CurrentSlotId = SlotId;
	CurrentUserIndex = UserIndex;

	return true;
}

bool UStorageSubsystem::UpdateStorage(FName SlotId, int UserIndex)
{
	if (!IsValid(CurrentStorage))
	{
		LOG_ERROR(LogTemp, TEXT("Storage is not valid"));
		return false;
	}

	FString SlotName = SlotId.ToString();

	if (!UGameplayStatics::SaveGameToSlot(CurrentStorage, SlotName, UserIndex))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to save storage to slot: %s"), *SlotName);
		return false;
	}

	LOG_INFO(LogTemp, TEXT("Storage updated and saved to slot: %s"), *SlotName);
	return true;
}

bool UStorageSubsystem::DoesStorageExist(FName SlotId, int UserIndex)
{
	return UGameplayStatics::DoesSaveGameExist(SlotId.ToString(), UserIndex);
}



USaveGame* UStorageSubsystem::GetLocalStorage()
{
	return CurrentStorage;
}



bool UStorageSubsystem::CreateNewStorage(FName SlotId, int UserIndex)
{
	USaveGame* NewSaveGame = UGameplayStatics::CreateSaveGameObject(UStorage::StaticClass());
	if (!IsValid(NewSaveGame))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to create save game object"));
		return false;
	}

	const FString SlotName = SlotId.ToString();

	if (!UGameplayStatics::SaveGameToSlot(NewSaveGame, SlotName, UserIndex))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to save new storage to slot: %s"), *SlotName);
		return false;
	}

	CurrentStorage = Cast<UStorage>(NewSaveGame);

	LOG_INFO(LogTemp, TEXT("New storage created and saved to slot: %s"), *SlotName);
	return true;
}

void UStorageSubsystem::HandleGameInitialized()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);
	if (ReadStorage())
	{
		FLatentDelegates::OnStorageLoaded.Broadcast();
	}
}

bool UStorageSubsystem::ShouldCreateSubsystem(UObject* Object) const
{
	return true;
}

void UStorageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!FLatentDelegates::OnPreGameInitialized.IsBoundToObject(this))
	{
		FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UStorageSubsystem::HandleGameInitialized);
	}
}

void UStorageSubsystem::Deinitialize()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	UpdateStorage();
	Super::Deinitialize();
}

