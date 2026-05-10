// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/TaskType.h"

// Generated Headers
#include "AuthAction.generated.h"

// Module Macros
#define REN_API RCOREAUTHACTION_API


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAuthAction : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<UAuthAction> NextNode;

	FOnTaskFinished OnActionFinished;


	/* TODO: chang return type to bool */
	REN_API bool StartAction();
	REN_API void StopAction();

	void Cleanup();

	FGuid GetActionId();
	void SetActionId(FGuid NewId);

protected:

	FGuid ActionId;

	/* TODO: change return type to bool */
	REN_API virtual void OnStarted();
	REN_API virtual void OnCompleted(bool bSuccess);
	REN_API virtual void OnCleanup();

	REN_API void Success();
	REN_API void Fail(const FString& Reason);

};


// Module Macros
#undef REN_API

