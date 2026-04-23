// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/PoolSubsystem.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


UObject* UPoolSubsystem::AcquireObject(UClass* Class, UObject* Outer)
{
	TArray<TObjectPtr<UObject>>& Pool = Pools.FindOrAdd(Class);
	if (Pool.Num() > 0)
	{
		return Pool.Pop();
	}
	return NewObject<UObject>(IsValid(Outer) ? Outer : this, Class);
}

void UPoolSubsystem::ReturnToPool(UObject* Item)
{
	if (!IsValid(Item))
	{
		return;
	}

	UClass* Class = Item->GetClass();
	TArray<TObjectPtr<UObject>>& Pool = Pools.FindOrAdd(Class);
	if (IsValid(Item))
	{
		Pool.Add(Item);
	}
}

bool UPoolSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogPool, TEXT("PoolSubsystem initialized"));
}

void UPoolSubsystem::Deinitialize()
{
	LOG_WARNING(LogPool, TEXT("PoolSubsystem deinitialized"));
	Super::Deinitialize();
}


UPoolSubsystem* UPoolSubsystem::Get(UWorld* World)
{
	return Get(World->GetGameInstance());
}

UPoolSubsystem* UPoolSubsystem::Get(UGameInstance* GameInstance)
{
	return GameInstance->GetSubsystem<UPoolSubsystem>();
}

