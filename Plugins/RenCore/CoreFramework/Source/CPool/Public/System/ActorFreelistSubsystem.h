// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Generated Headers
#include "ActorFreelistSubsystem.generated.h"


/**
 * 
 */
UCLASS(MinimalAPI)
class UActorFreelistSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	CPOOL_API AActor* AcquireFromList(TSubclassOf<AActor> ActorClass, const FTransform& Transform, AActor* Owner);
	CPOOL_API void ReturnToList(AActor* Actor);
	CPOOL_API void ClearList();

	template<class T>
	T* AcquireFromList(TSubclassOf<AActor> ActorClass, const FTransform& Transform, AActor* Owner)
	{
		return Cast<T>(AcquireFromList(ActorClass, Transform, Owner));
	}

protected:

	UPROPERTY()
	TMap<TSubclassOf<AActor>, TObjectPtr<AActor>> ActorList;


	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

public:

	static CPOOL_API UActorFreelistSubsystem* Get(UWorld* World);

};

