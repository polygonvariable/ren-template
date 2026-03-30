// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "TaskSubsystem.generated.h"

// Module Macros
#define REN_API RCORETASK_API

// Forward Declarations
class UTaskObject;



/**
 *
 *
 *
 */
UCLASS(MinimalAPI, NotBlueprintType)
class UTaskSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	REN_API UTaskObject* CreateTask(FGuid TaskId, TSubclassOf<UTaskObject> TaskClass);

	template<class T>
	T* CreateTask(FGuid TaskId)
	{
		return Cast<T>(CreateTask(TaskId, T::StaticClass()));
	}

	REN_API void StartTask(FGuid TaskId);
	REN_API void StopTask(FGuid TaskId);

protected:

	UPROPERTY()
	TMap<FGuid, TObjectPtr<UTaskObject>> ActiveTasks;

	/* A free list of latent tasks */
	UPROPERTY()
	TMap<TSubclassOf<UTaskObject>, TObjectPtr<UTaskObject>> TaskList;


	UTaskObject* AcquireFromList(TSubclassOf<UTaskObject> TaskClass);
	void ReturnToList(UTaskObject* Task);

	UTaskObject* FindTask(FGuid TaskId);

	void HandleOnTaskFinished(FGuid TaskId);

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	REN_API static UTaskSubsystem* Get(UWorld* World);
	REN_API static UTaskSubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef REN_API

