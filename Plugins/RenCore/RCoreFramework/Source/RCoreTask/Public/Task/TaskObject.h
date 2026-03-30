// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/TaskType.h"

// Generated Headers
#include "TaskObject.generated.h"

// Module Macros
#define REN_API RCORETASK_API

// Forward Declarations



/**
 *
 *
 *
 */
UCLASS(Abstract, MinimalAPI)
class UTaskObject : public UObject
{

	GENERATED_BODY()

public:

	FTaskCallback Callback;

	REN_API void StartTask();
	REN_API void StopTask();

	void Cleanup();

	FOnTaskFinished& GetOnTaskFinished();

	FGuid GetTaskId();
	void SetTaskId(FGuid NewId);

	UPROPERTY()
	TObjectPtr<UTaskObject> NextNode;

protected:

	FOnTaskFinished OnFinished;
	FGuid TaskId;

	REN_API virtual void OnStarted() {};
	REN_API virtual void OnStopped() {};
	REN_API virtual void OnCleanup() {};

	REN_API void Success();
	REN_API void Fail(const FString& Reason);

};

UCLASS(Abstract, MinimalAPI)
class UNetworkTaskObject : public UTaskObject
{

	GENERATED_BODY()

};



// Module Macros
#undef REN_API

