// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "TimestampSubsystem.h"

// Engine Headers

// Project Headers
#include "RenCore/Public/Delegate/LatentDelegates.h"
#include "RenCore/Public/Interface/StorageProviderInterface.h"

#include "RenCoreLibrary/Public/LogMacro.h"
#include "RenCoreLibrary/Public/SubsystemUtils.h"

#include "RenCoreTimestamp/Public/TimestampProviderInterface.h"



bool UTimestampSubsystem::AddTimestamp(FName TimestampId, bool bForceAdd)
{
	ITimestampProviderInterface* TimestampProvider = TimestampInterface.Get();
	if (!TimestampProvider)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("TimestampInterface is invalid"));
		return false;
	}

	TMap<FName, FDateTime>& Timestamp = TimestampProvider->GetMutableTimestamp();
	if (Timestamp.Contains(TimestampId) && !bForceAdd)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Timestamp already exists and bForceAdd is false"));
		return false;
	}

	Timestamp.Add(TimestampId, FDateTime::Now());

	PRINT_SUCCESS(LogTemp, 1.0f, TEXT("Timestamp added"));

	return true;
}

bool UTimestampSubsystem::RemoveTimestamp(FName TimestampId)
{
	ITimestampProviderInterface* TimestampProvider = TimestampInterface.Get();
	if (!TimestampProvider)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("TimestampProvider is invalid"));
		return false;
	}

	TMap<FName, FDateTime>& Timestamp = TimestampProvider->GetMutableTimestamp();
	if (Timestamp.Remove(TimestampId) > 0)
	{
		PRINT_SUCCESS(LogTemp, 1.0f, TEXT("Timestamp removed"));
		return true;
	}

	return false;
}

bool UTimestampSubsystem::ContainsTimestamp(FName TimestampId) const
{
	ITimestampProviderInterface* TimestampProvider = TimestampInterface.Get();
	if (TimestampProvider)
	{
		return TimestampProvider->GetTimestamp().Contains(TimestampId);
	}
	return false;
}

const FDateTime* UTimestampSubsystem::GetTimestamp(FName TimestampId) const
{
	ITimestampProviderInterface* TimestampProvider = TimestampInterface.Get();
	if (!TimestampProvider)
	{
		return nullptr;
	}
	
	const TMap<FName, FDateTime>& Timestamp = TimestampProvider->GetTimestamp();
	return Timestamp.Find(TimestampId);
}

TEnumAsByte<ETimestampStatus> UTimestampSubsystem::GetStatus(FName TimestampId) const
{
	return ContainsTimestamp(TimestampId) ? ETimestampStatus::Changed : ETimestampStatus::Unchanged;
}

TEnumAsByte<ETimestampCooldownStatus> UTimestampSubsystem::GetCooldownStatus(FName TimestampId, bool bOnlyOnce, const FTimespan& CooldownTime) const
{
	const FDateTime* Date = GetTimestamp(TimestampId);
	if (!Date)
	{
		return ETimestampCooldownStatus::NotFound;
	}

	if (bOnlyOnce)
	{
		return ETimestampCooldownStatus::Once;
	}

	if (FDateTime::Now() >= (*Date + CooldownTime))
	{
		return ETimestampCooldownStatus::Completed;
	}
	else
	{
		return ETimestampCooldownStatus::Pending;
	}
}



void UTimestampSubsystem::HandleStorageLoaded()
{
	FLatentDelegates::OnStorageLoaded.RemoveAll(this);

	IStorageProviderInterface* StorageProvider = SubsystemUtils::GetSubsystemInterface<IStorageProviderInterface>(GetGameInstance());
	if (!StorageProvider)
	{
		LOG_ERROR(LogTemp, TEXT("StorageProvider is invalid"));
		return;
	}

	UObject* Storage = StorageProvider->GetLocalStorage();
	ITimestampProviderInterface* TimestampProvider = Cast<ITimestampProviderInterface>(Storage);
	if (!TimestampProvider)
	{
		LOG_ERROR(LogTemp, TEXT("TimestampProvider is invalid"));
		return;
	}

	TimestampInterface = TWeakInterfacePtr<ITimestampProviderInterface>(TimestampProvider);
}


void UTimestampSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("Timestamp Initialized"));

	FLatentDelegates::OnStorageLoaded.AddUObject(this, &UTimestampSubsystem::HandleStorageLoaded);
}

void UTimestampSubsystem::Deinitialize()
{
	FLatentDelegates::OnStorageLoaded.RemoveAll(this);

	TimestampInterface.Reset();

	LOG_WARNING(LogTemp, TEXT("Timestamp Deinitialized"));
	Super::Deinitialize();
}

