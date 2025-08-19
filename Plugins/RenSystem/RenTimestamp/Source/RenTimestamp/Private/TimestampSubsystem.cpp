// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "TimestampSubsystem.h"

// Engine Headers

// Project Headers
#include "RenCore/Public/Interface/StorageProviderInterface.h"
#include "RenCore/Public/Delegate/LatentDelegates.h"
#include "RenCoreLibrary/Public/SubsystemUtils.h"
#include "RenCoreLibrary/Public/LogMacro.h"

#include "RenCoreTimestamp/Public/TimestampProviderInterface.h"



bool UTimestampSubsystem::AddTimestamp(FName Guid, bool bForceAdd)
{
	ITimestampProviderInterface* TimestampProvider = TimestampInterface.Get();
	if (!TimestampProvider)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("TimestampInterface is invalid"));
		return false;
	}

	if (ContainsTimestamp(Guid) && !bForceAdd)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT(" already exists and bForceAdd is false"));
		return false;
	}

	TMap<FName, FDateTime>& Timestamp = TimestampProvider->GetMutableTimestamp();
	Timestamp.Add(Guid, FDateTime::Now());

	PRINT_INFO(LogTemp, 1.0f, TEXT(" added"));

	return true;
}

bool UTimestampSubsystem::RemoveTimestamp(FName Guid)
{
	ITimestampProviderInterface* TimestampProvider = TimestampInterface.Get();
	if (!TimestampProvider)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("TimestampProvider is invalid"));
		return false;
	}

	TMap<FName, FDateTime>& Timestamp = TimestampProvider->GetMutableTimestamp();
	if (Timestamp.Remove(Guid) > 0)
	{
		PRINT_INFO(LogTemp, 1.0f, TEXT(" removed"));
		return true;
	}

	return false;
}

bool UTimestampSubsystem::ContainsTimestamp(FName Guid) const
{
	ITimestampProviderInterface* TimestampProvider = TimestampInterface.Get();
	if (TimestampProvider)
	{
		return TimestampProvider->GetTimestamp().Contains(Guid);
	}
	return false;
}

const FDateTime* UTimestampSubsystem::GetTimestamp(FName Guid) const
{
	ITimestampProviderInterface* TimestampProvider = TimestampInterface.Get();
	if (!TimestampProvider)
	{
		return nullptr;
	}
	
	const TMap<FName, FDateTime>& Timestamp = TimestampProvider->GetTimestamp();
	return Timestamp.Find(Guid);
}

TEnumAsByte<ETimestampStatus> UTimestampSubsystem::GetStatus(FName Guid) const
{
	return ContainsTimestamp(Guid) ? ETimestampStatus::Changed : ETimestampStatus::Unchanged;
}

TEnumAsByte<ETimestampCooldownStatus> UTimestampSubsystem::GetCooldownStatus(FName Guid, bool bOnlyOnce, const FTimespan& CooldownTime) const
{
	const FDateTime* Date = GetTimestamp(Guid);
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

