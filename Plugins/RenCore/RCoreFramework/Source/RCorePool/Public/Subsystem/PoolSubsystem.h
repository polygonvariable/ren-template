// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "PoolSubsystem.generated.h"

// Module Macros
#define REN_API RCOREPOOL_API


/**
 *
 */
UCLASS(MinimalAPI)
class UPoolSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	REN_API UObject* AcquireObject(UClass* Class, UObject* Outer = nullptr);
	REN_API void ReturnToPool(UObject* Item);

	template<typename T>
	T* AcquireObject(UObject* Outer = nullptr)
	{
		return Cast<T>(AcquireObject(T::StaticClass(), Outer));
	}

	template<typename T>
	T* AcquireObject(UClass* Class, UObject* Outer = nullptr)
	{
		return Cast<T>(AcquireObject(Class, Outer));
	}

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

	static REN_API UPoolSubsystem* Get(UWorld* World);
	static REN_API UPoolSubsystem* Get(UGameInstance* GameInstance);

protected:

	TMap<UClass*, TArray<TObjectPtr<UObject>>> Pools;

};


// Module Macros
#undef REN_API

