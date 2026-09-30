// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "AuthAction.generated.h"


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAuthAction : public UObject
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE_OneParam(FOnActionFinished, FGuid);
	FOnActionFinished OnActionFinished;

	UPROPERTY()
	TObjectPtr<UAuthAction> NextNode;


	/* TODO: chang return type to bool */
	CAUTHACTION_API bool StartAction();
	CAUTHACTION_API void StopAction();

	void Cleanup();

	FGuid GetActionId();
	void SetActionId(FGuid NewId);

protected:

	FGuid ActionId;

	/* TODO: change return type to bool */
	CAUTHACTION_API virtual void OnStarted();
	CAUTHACTION_API virtual void OnCompleted(bool bSuccess);
	CAUTHACTION_API virtual void OnCleanup();

	CAUTHACTION_API void Success();
	CAUTHACTION_API void Fail(const FString& Reason);

};

