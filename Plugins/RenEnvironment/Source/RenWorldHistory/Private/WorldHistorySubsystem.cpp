// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WorldHistorySubsystem.h"

// Engine Headers
#include "GameFramework/SaveGame.h"

// Project Headers
#include "RenCore/Public/Interface/WorldHistoryProviderInterface.h"
#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"
#include "RenCore/Public/Interface/StorageProviderInterface.h"



bool UWorldHistorySubsystem::AddHistory(const FName Id, const bool bForceAdd)
{
	IWorldHistoryProviderInterface* WorldHistoryInterfacePtr = WorldHistoryInterface.Get();
	if (!WorldHistoryInterfacePtr)
	{
		LOG_ERROR(LogTemp, "WorldHistoryInterfacePtr is invalid");
		return false;
	}

	if (ContainsHistory(Id) && !bForceAdd)
	{
		LOG_ERROR(LogTemp, "History already exists and bForceAdd is false");
		return false;
	}

	TMap<FName, FDateTime>& WorldHistory = WorldHistoryInterfacePtr->GetMutableWorldHistory();
	WorldHistory.Add(Id, FDateTime::Now());

	return true;
}

bool UWorldHistorySubsystem::RemoveHistory(const FName Id)
{
	IWorldHistoryProviderInterface* WorldHistoryInterfacePtr = WorldHistoryInterface.Get();
	if (!WorldHistoryInterfacePtr)
	{
		LOG_ERROR(LogTemp, "WorldHistoryInterfacePtr is invalid");
		return false;
	}

	TMap<FName, FDateTime>& WorldHistory = WorldHistoryInterfacePtr->GetMutableWorldHistory();
	WorldHistory.Remove(Id);

	return true;
}

bool UWorldHistorySubsystem::ContainsHistory(const FName Id) const
{
	IWorldHistoryProviderInterface* WorldHistoryInterfacePtr = WorldHistoryInterface.Get();
	if (WorldHistoryInterfacePtr)
	{
		return WorldHistoryInterfacePtr->GetWorldHistory().Contains(Id);
	}
	return false;
}

bool UWorldHistorySubsystem::GetHistoryDate(const FName Id, FDateTime& Date) const
{
	IWorldHistoryProviderInterface* WorldHistoryInterfacePtr = WorldHistoryInterface.Get();
	if (WorldHistoryInterfacePtr)
	{
		const TMap<FName, FDateTime>& WorldHistory = WorldHistoryInterfacePtr->GetWorldHistory();
		if (WorldHistory.Contains(Id))
		{
			Date = WorldHistory.FindRef(Id);
			return true;
		}
	}
	return false;
}

TEnumAsByte<EWorldHistoryStatus> UWorldHistorySubsystem::GetHistoryStatus(const FName Id) const
{
	return ContainsHistory(Id) ? EWorldHistoryStatus::Changed : EWorldHistoryStatus::Unchanged;
}

TEnumAsByte<EWorldHistoryCooldownStatus> UWorldHistorySubsystem::GetHistoryCooldownStatus(const FName Id, const bool bIsOnlyOnce, const FTimespan CooldownTime) const
{
	FDateTime Date;
	if (GetHistoryDate(Id, Date))
	{
		if (bIsOnlyOnce)
		{
			return EWorldHistoryCooldownStatus::Once;
		}
		else
		{
			if (FDateTime::Now() >= (Date + CooldownTime))
			{
				return EWorldHistoryCooldownStatus::Completed;
			}
			else
			{
				return EWorldHistoryCooldownStatus::Pending;
			}
		}
	}

	return EWorldHistoryCooldownStatus::NotFound;
}

bool UWorldHistorySubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UWorldHistorySubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_WARNING(LogTemp, TEXT("WorldHistory WorldComponentsUpdated"));


	UGameInstance* GameInstance = InWorld.GetGameInstance();
	if (!IsValid(GameInstance))
	{
		LOG_ERROR(LogTemp, TEXT("GameInstance is invalid"));
		return;
	}

	IStorageProviderInterface* StorageInterface = SubsystemUtils::GetSubsystemInterface<UGameInstance, UGameInstanceSubsystem, IStorageProviderInterface>(GameInstance);
	if (!StorageInterface)
	{
		LOG_ERROR(LogTemp, TEXT("StorageInterface is invalid"));
		return;
	}

	USaveGame* SaveGame = StorageInterface->GetLocalStorage();
	if (IsValid(SaveGame) && SaveGame->Implements<UWorldHistoryProviderInterface>())
	{
		IWorldHistoryProviderInterface* WorldHistoryInterfacePtr = Cast<IWorldHistoryProviderInterface>(SaveGame);
		if (!WorldHistoryInterfacePtr)
		{
			LOG_ERROR(LogTemp, TEXT("WorldHistoryInterfacePtr is invalid"));
			return;
		}
		WorldHistoryInterface = TWeakInterfacePtr<IWorldHistoryProviderInterface>(WorldHistoryInterfacePtr);
	}
}

void UWorldHistorySubsystem::Deinitialize()
{
	LOG_WARNING(LogTemp, TEXT("WorldHistory Deinitialized"));
	Super::Deinitialize();
}

