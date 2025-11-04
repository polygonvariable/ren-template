// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "PoolSubsystem.h"

// Engine Headers

// Project Headers
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"



UObject* UPoolSubsystem::AcquireObject(UClass* Class)
{
	TArray<TObjectPtr<UObject>>& Pool = Pools.FindOrAdd(Class);
	if (Pool.Num() > 0)
	{
		return Pool.Pop();
	}
	return NewObject<UObject>(this, Class);
}

void UPoolSubsystem::ReturnToPool(UClass* Class, UObject* Item)
{
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
	LOG_WARNING(LogPool, TEXT("Initialized"));
}

void UPoolSubsystem::Deinitialize()
{
	LOG_WARNING(LogPool, TEXT("Deinitialized"));
	Super::Deinitialize();
}

