// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventTimestampSubsystem.h"

// Engine Headers

// Project Headers
#include "RenCore/Public/Interface/StorageProviderInterface.h"
#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"
#include "RenCore/Public/EventTimestamp/EventTimestampProviderInterface.h"



bool UEventTimestampSubsystem::AddHistory(FName Guid, bool bForceAdd)
{
	IEventTimestampProviderInterface* EventTimestampProvider = EventTimestampInterface.Get();
	if (!EventTimestampProvider)
	{
		LOG_ERROR(LogTemp, TEXT("EventTimestampInterface is invalid"));
		return false;
	}

	if (ContainsHistory(Guid) && !bForceAdd)
	{
		LOG_ERROR(LogTemp, TEXT("History already exists and bForceAdd is false"));
		return false;
	}

	TMap<FName, FDateTime>& EventTimestamp = EventTimestampProvider->GetMutableEventTimestamp();
	EventTimestamp.Add(Guid, FDateTime::Now());

	return true;
}

bool UEventTimestampSubsystem::RemoveHistory(FName Guid)
{
	IEventTimestampProviderInterface* EventTimestampProvider = EventTimestampInterface.Get();
	if (!EventTimestampProvider)
	{
		LOG_ERROR(LogTemp, TEXT("EventTimestampProvider is invalid"));
		return false;
	}

	TMap<FName, FDateTime>& EventTimestamp = EventTimestampProvider->GetMutableEventTimestamp();
	EventTimestamp.Remove(Guid);

	return true;
}

bool UEventTimestampSubsystem::ContainsHistory(FName Guid) const
{
	IEventTimestampProviderInterface* EventTimestampProvider = EventTimestampInterface.Get();
	if (EventTimestampProvider)
	{
		return EventTimestampProvider->GetEventTimestamp().Contains(Guid);
	}
	return false;
}

bool UEventTimestampSubsystem::GetHistoryDate(FName Guid, FDateTime& Date) const
{
	IEventTimestampProviderInterface* EventTimestampProvider = EventTimestampInterface.Get();
	if (EventTimestampProvider)
	{
		const TMap<FName, FDateTime>& EventTimestamp = EventTimestampProvider->GetEventTimestamp();
		if (EventTimestamp.Contains(Guid))
		{
			Date = EventTimestamp.FindRef(Guid);
			return true;
		}
	}
	return false;
}

TEnumAsByte<EEventTimestampStatus> UEventTimestampSubsystem::GetHistoryStatus(FName Guid) const
{
	return ContainsHistory(Guid) ? EEventTimestampStatus::Changed : EEventTimestampStatus::Unchanged;
}

TEnumAsByte<EEventTimestampCooldownStatus> UEventTimestampSubsystem::GetHistoryCooldownStatus(FName Guid, bool bOnlyOnce, const FTimespan& CooldownTime) const
{
	FDateTime Date;
	if (GetHistoryDate(Guid, Date))
	{
		if (bOnlyOnce)
		{
			return EEventTimestampCooldownStatus::Once;
		}
		else
		{
			if (FDateTime::Now() >= (Date + CooldownTime))
			{
				return EEventTimestampCooldownStatus::Completed;
			}
			else
			{
				return EEventTimestampCooldownStatus::Pending;
			}
		}
	}

	return EEventTimestampCooldownStatus::NotFound;
}



bool UEventTimestampSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UEventTimestampSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	LOG_WARNING(LogTemp, TEXT("EventTimestamp WorldComponentsUpdated"));


	UGameInstance* GameInstance = InWorld.GetGameInstance();
	if (!IsValid(GameInstance))
	{
		LOG_ERROR(LogTemp, TEXT("GameInstance is invalid"));
		return;
	}

	IStorageProviderInterface* StorageProvider = SubsystemUtils::GetSubsystemInterface<IStorageProviderInterface>(GameInstance);
	if (!StorageProvider)
	{
		LOG_ERROR(LogTemp, TEXT("StorageProvider is invalid"));
		return;
	}

	UObject* SaveGame = StorageProvider->GetLocalStorage();
	if (IsValid(SaveGame) && SaveGame->Implements<UEventTimestampProviderInterface>())
	{
		IEventTimestampProviderInterface* EventTimestampProvider = Cast<IEventTimestampProviderInterface>(SaveGame);
		if (!EventTimestampProvider)
		{
			LOG_ERROR(LogTemp, TEXT("EventTimestampProvider is invalid"));
			return;
		}
		EventTimestampInterface = TWeakInterfacePtr<IEventTimestampProviderInterface>(EventTimestampProvider);
	}
}

void UEventTimestampSubsystem::Deinitialize()
{
	EventTimestampInterface.Reset();

	LOG_WARNING(LogTemp, TEXT("EventTimestamp Deinitialized"));
	Super::Deinitialize();
}

