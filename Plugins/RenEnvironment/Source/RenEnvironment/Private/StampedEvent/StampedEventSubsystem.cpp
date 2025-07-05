// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "StampedEvent/StampedEventSubsystem.h"

// Project Headers
#include "RenCore/Public/Macro/GameInstanceMacro.h"
#include "RenCore/Public/Macro/LogMacro.h"
//#include "RenStorage/Public/Storage.h"
//#include "RenStorage/Public/StorageSubsystem.h"


bool UStampedEventSubsystem::AddChange_Implementation(const FName Id, const bool bForceAdd)
{
	/*if (!IsValid(Storage)) {
		LOG_ERROR(LogTemp, "Storage is null");
		return false;
	}

	if (HasChanged(Id) && !bForceAdd) {
		LOG_ERROR(LogTemp, "Stamped event already exists");
		return false;
	}

	Storage->StampedEvents.Add(Id, FDateTime::Now());
	LOG_INFO(LogTemp, "Stamped event added");*/
	return true;
}

bool UStampedEventSubsystem::RemoveChange_Implementation(const FName Id)
{
	/*if (!IsValid(Storage)) {
		LOG_ERROR(LogTemp, "Storage is null");
		return false;
	}
	Storage->StampedEvents.Remove(Id);
	LOG_INFO(LogTemp, "Stamped event removed");*/
	return true;
}

bool UStampedEventSubsystem::HasChanged_Implementation(const FName Id)
{
	/*if (IsValid(Storage)) {
		return Storage->StampedEvents.Contains(Id);
	}*/
	return false;
}

bool UStampedEventSubsystem::GetEventDate_Implementation(const FName Id, FDateTime& EventDate)
{
	/*if (IsValid(Storage)) {
		if (Storage->StampedEvents.Contains(Id)) {
			EventDate = Storage->StampedEvents.FindChecked(Id);
			return true;
		}
	}*/
	return false;
}

EStampedEventStatus UStampedEventSubsystem::GetEventStatus_Implementation(const FName Id)
{
	return HasChanged(Id) ? EStampedEventStatus::Changed : EStampedEventStatus::Unchanged;
}

EStampedEventCooldownStatus UStampedEventSubsystem::GetEventCooldownStatus_Implementation(const FName Id, const bool bIsOnlyOnce, const FTimespan CooldownTime)
{
	if (HasChanged(Id)) {

		FDateTime EventDate;
		GetEventDate(Id, EventDate);

		if (bIsOnlyOnce) {
			return EStampedEventCooldownStatus::Once;
		}
		else {
			if (FDateTime::Now() >= (EventDate + CooldownTime)) {
				return EStampedEventCooldownStatus::Completed;
			}
			else {
				return EStampedEventCooldownStatus::Pending;
			}
		}
	}

	return EStampedEventCooldownStatus::NotFound;
}

//void UStampedEventSubsystem::PostInitialize_Implementation()
//{
//	Super::PostInitialize_Implementation();
//
//	UStorageSubsystem* StorageSubsystem = nullptr;
//	GET_GAMEINSTANCESUBSYSTEM_FROM_GAMEINSTANCE(UStorageSubsystem, StorageSubsystem);
//
//	Storage = StorageSubsystem->GetLocalStorage();
//	if (!IsValid(Storage))
//	{
//		LOG_ERROR(LogTemp, "LocalStorage not found");
//	}
//}

