// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers

// Generated Headers
#include "PoolSubsystem.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(MinimalAPI)
class UPoolSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	RCOREPOOL_API UObject* AcquireObject(UClass* Class);
	RCOREPOOL_API void ReturnToPool(UClass* Class, UObject* Item);

	template<typename T>
	T* AcquireObject()
	{
		return Cast<T>(AcquireObject(T::StaticClass()));
	}

	template<typename T>
	void ReturnToPool(T* Item)
	{
		ReturnToPool(T::StaticClass(), Item);
	}

protected:

	TMap<UClass*, TArray<TObjectPtr<UObject>>> Pools;

public:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

};

