// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "CounterSubsystem.h"

// Engine Headers

// Project Headers
#include "RCoreDelegate/Public/LatentDelegates.h"

#include "RCoreCounter/Public/CounterContainer.h"
#include "RCoreCounter/Public/CounterProviderInterface.h"

#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreStorage/Public/StorageUtils.h"



int UCounterSubsystem::AddCounter(FName Name, bool bAutoIncrement, int Max)
{
	bool bFound = LocalCounters.Contains(Name);
	if (bFound)
	{
		if (!bAutoIncrement)
		{
			PRINT_ERROR(LogCounter, 1.0f, TEXT("Counter already exists"));
			return -1;
		}

		return IncrementCounter(Name, Max);
	}

	LocalCounters.Add(Name, 0);
	return 0;
}

bool UCounterSubsystem::RemoveCounter(FName Name)
{
	int Count = LocalCounters.Remove(Name);
	return Count > 0;
}

int UCounterSubsystem::IncrementCounter(FName Name, int Max)
{
	int* Counter = LocalCounters.Find(Name);
	if (!Counter)
	{
		PRINT_ERROR(LogCounter, 1.0f, TEXT("Counter does not exist"));
		return -1;
	}

	*Counter = FMath::Clamp((*Counter + 1), 0, Max);
	return *Counter;
}

int UCounterSubsystem::DecrementCounter(FName Name)
{
	int* Counter = LocalCounters.Find(Name);
	if (!Counter)
	{
		PRINT_ERROR(LogCounter, 1.0f, TEXT("Counter does not exist"));
		return -1;
	}

	*Counter = FMath::Max((*Counter - 1), 0);
	return *Counter;
}

bool UCounterSubsystem::ContainsCounter(FName Name) const
{
	return LocalCounters.Contains(Name);
}

int UCounterSubsystem::GetCounter(FName Name) const
{
	const int* Counter = LocalCounters.Find(Name);
	if (!Counter)
	{
		return -1;
	}
	return *Counter;
}



int UCounterSubsystem::AddCounter(FName ContainerId, FName Name, bool bAutoIncrement, int Max)
{
	TMap<FName, FCounterRecord>* Counters = FindOrAddCounters(ContainerId);
	if (!Counters || !Name.IsValid())
	{
		PRINT_ERROR(LogCounter, 1.0f, TEXT("Counter container does not exist"));
		return -1;
	}

	bool bFound = Counters->Contains(Name);
	if (bFound)
	{
		if (!bAutoIncrement)
		{
			PRINT_ERROR(LogCounter, 1.0f, TEXT("Counter already exists"));
			return -1;
		}

		return IncrementCounter(ContainerId, Name, Max);
	}

	Counters->Add(Name, { 0, FDateTime::UtcNow() });
	return 0;
}

bool UCounterSubsystem::RemoveCounter(FName ContainerId, FName Name)
{
	TMap<FName, FCounterRecord>* Counters = FindOrAddCounters(ContainerId);
	if (!Counters)
	{
		PRINT_ERROR(LogCounter, 1.0f, TEXT("Counter container does not exist"));
		return false;
	}

	int Count = Counters->Remove(Name);
	return Count > 0;
}

bool UCounterSubsystem::ResetCounter(FName ContainerId)
{
	TMap<FName, FCounterRecord>* Counters = FindOrAddCounters(ContainerId);
	if (!Counters)
	{
		return false;
	}

	Counters->Empty();
	return true;
}

int UCounterSubsystem::IncrementCounter(FName ContainerId, FName Name, int Max)
{
	FCounterRecord* Counter = GetCounterRecordX(ContainerId, Name);
	if (!Counter)
	{
		PRINT_ERROR(LogCounter, 1.0f, TEXT("Counter does not exist"));
		return -1;
	}

	int NewValue = Counter->Value + 1;

	Counter->Value = FMath::Clamp(NewValue, 0, Max);
	Counter->Timestamp = FDateTime::UtcNow();

	return Counter->Value;
}

int UCounterSubsystem::DecrementCounter(FName ContainerId, FName Name)
{
	FCounterRecord* Counter = GetCounterRecordX(ContainerId, Name);
	if (!Counter)
	{
		PRINT_ERROR(LogCounter, 1.0f, TEXT("Counter does not exist"));
		return -1;
	}

	int NewValue = Counter->Value - 1;

	Counter->Value = FMath::Max(NewValue, 0);
	Counter->Timestamp = FDateTime::UtcNow();

	return Counter->Value;
}

bool UCounterSubsystem::ContainsCounter(FName ContainerId, FName Name) const
{
	const TMap<FName, FCounterRecord>* Counters = GetCounters(ContainerId);
	if (!Counters)
	{
		return false;
	}
	return Counters->Contains(Name);
}

const FCounterRecord* UCounterSubsystem::GetCounterRecord(FName ContainerId, FName Name) const
{
	const TMap<FName, FCounterRecord>* Counters = GetCounters(ContainerId);
	if (!Counters)
	{
		return nullptr;
	}
	return Counters->Find(Name);
}

const FCounterRecord* UCounterSubsystem::FindOrAddCounter(FName ContainerId, FName Name)
{
	if (!Name.IsValid())
	{
		return nullptr;
	}

	TMap<FName, FCounterRecord>* Counters = FindOrAddCounters(ContainerId);
	if (!Counters)
	{
		return nullptr;
	}

	FCounterRecord& Counter = Counters->FindOrAdd(Name, { 0, FDateTime::UtcNow() });
	return &Counter;
}




TMap<FName, FCounterRecord>* UCounterSubsystem::FindOrAddCounters(FName ContainerId)
{
	if (!ContainerId.IsValid())
	{
		return nullptr;
	}

	ICounterProviderInterface* Counter = CounterProvider.Get();
	if (!Counter)
	{
		return nullptr;
	}

	TMap<FName, FCounterContainer>& Containers = Counter->GetCounterContainer();
	FCounterContainer& Container = Containers.FindOrAdd(ContainerId, { FGuid::NewGuid() });

	TMap<FName, FCounterRecord>* Counters = &Container.Counters;

	return Counters;
}

const TMap<FName, FCounterRecord>* UCounterSubsystem::GetCounters(FName ContainerId) const
{
	const ICounterProviderInterface* Counter = CounterProvider.Get();
	if (!Counter)
	{
		return nullptr;
	}

	const TMap<FName, FCounterContainer>& Containers = Counter->GetCounterContainer();
	const FCounterContainer* Container = Containers.Find(ContainerId);
	if (!Container)
	{
		return nullptr;
	}

	return &Container->Counters;
}

FCounterRecord* UCounterSubsystem::GetCounterRecordX(FName ContainerId, FName Name)
{
	TMap<FName, FCounterRecord>* Counters = FindOrAddCounters(ContainerId);
	if (!Counters)
	{
		return nullptr;
	}
	return Counters->Find(Name);
}



void UCounterSubsystem::HandleStorageLoaded()
{
	FLatentDelegates::OnStorageLoaded.RemoveAll(this);
	LOG_INFO(LogInventory, TEXT("CounterSubsystem storage loading"));

	ICounterProviderInterface* Counter = StorageUtils::GetStorageInterface<ICounterProviderInterface>(GetGameInstance());
	if (!Counter)
	{
		LOG_ERROR(LogInventory, TEXT("CounterProvider is invalid"));
		return;
	}

	CounterProvider = TWeakInterfacePtr<ICounterProviderInterface>(Counter);

	LOG_INFO(LogInventory, TEXT("CounterSubsystem storage loaded"));
}

bool UCounterSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UCounterSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogCounter, TEXT("Initialize"));

	if (!FLatentDelegates::OnStorageLoaded.IsBoundToObject(this))
	{
		FLatentDelegates::OnStorageLoaded.AddUObject(this, &UCounterSubsystem::HandleStorageLoaded);
	}
}

void UCounterSubsystem::Deinitialize()
{
	LOG_WARNING(LogCounter, TEXT("Deinitialize"));
	Super::Deinitialize();
}

