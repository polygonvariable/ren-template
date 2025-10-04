// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "TimestampSubsystem.h"

// Engine Headers

// Project Headers
#include "RCoreDelegate/Public/LatentDelegates.h"

#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenCounter/Public/CounterSubsystem.h"
#include "RCoreCounter/Public/CounterContainer.h"


bool UTimestampSubsystem::AddTimestamp(FName ContainerId, FName TimestampId, bool bOverride)
{
	UCounterSubsystem* Counter = CounterSubsystem.Get();
	if (!IsValid(Counter))
	{
		LOG_ERROR(LogTimestamp, TEXT("CounterSubsystem is invalid"));
		return false;
	}

	int Result = Counter->AddCounter(ContainerId, TimestampId, bOverride);
	return Result > 0;
}

bool UTimestampSubsystem::RemoveTimestamp(FName ContainerId, FName TimestampId)
{
	UCounterSubsystem* Counter = CounterSubsystem.Get();
	if (!IsValid(Counter))
	{
		LOG_ERROR(LogTimestamp, TEXT("CounterSubsystem is invalid"));
		return false;
	}

	return Counter->RemoveCounter(ContainerId, TimestampId);
}

bool UTimestampSubsystem::ContainsTimestamp(FName ContainerId, FName TimestampId) const
{
	UCounterSubsystem* Counter = CounterSubsystem.Get();
	if (!IsValid(Counter))
	{
		return false;
	}

	return Counter->ContainsCounter(ContainerId, TimestampId);
}

FDateTime UTimestampSubsystem::GetTimestamp(FName ContainerId, FName TimestampId) const
{
	UCounterSubsystem* Counter = CounterSubsystem.Get();
	if (!IsValid(Counter))
	{
		return FDateTime::MinValue();
	}

	const FCounterRecord* Record = Counter->GetCounterRecord(ContainerId, TimestampId);
	if (!Record)
	{
		return FDateTime::MinValue();
	}

	return Record->Timestamp;
}

ETimestampStatus UTimestampSubsystem::GetStatus(FName ContainerId, FName TimestampId) const
{
	return ContainsTimestamp(ContainerId, TimestampId) ? ETimestampStatus::Changed : ETimestampStatus::Unchanged;
}

ETimestampCooldownStatus UTimestampSubsystem::GetCooldownStatus(FName ContainerId, FName TimestampId, bool bOnlyOnce, const FTimespan& CooldownTime) const
{
	 FDateTime Date = GetTimestamp(ContainerId, TimestampId);

	if (bOnlyOnce)
	{
		return ETimestampCooldownStatus::Once;
	}

	if (FDateTime::Now() >= (Date + CooldownTime))
	{
		return ETimestampCooldownStatus::Completed;
	}
	else
	{
		return ETimestampCooldownStatus::Pending;
	}
}



void UTimestampSubsystem::HandleGameInitialized()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	CounterSubsystem = GetGameInstance()->GetSubsystem<UCounterSubsystem>();
}

bool UTimestampSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UTimestampSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTimestamp, TEXT("Initialized"));

	FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UTimestampSubsystem::HandleGameInitialized);
}

void UTimestampSubsystem::Deinitialize()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	CounterSubsystem.Reset();

	LOG_WARNING(LogTimestamp, TEXT("Deinitialized"));
	Super::Deinitialize();
}

