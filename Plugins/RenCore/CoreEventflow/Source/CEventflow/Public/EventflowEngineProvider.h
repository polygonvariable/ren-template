// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "EventflowEngineProvider.generated.h"

// Forward Declarations
class UEventflowEngine;

// Delegate Declarations
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEvenflowEngineAdded, FPrimaryAssetId /* AssetId */, UEventflowEngine* /* Engine */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEventflowEngineRemoved, FPrimaryAssetId /* AssetId */, UEventflowEngine* /* Engine */);


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UEventflowEngineProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class CEVENTFLOW_API IEventflowEngineProvider
{

	GENERATED_BODY()

public:

	FOnEvenflowEngineAdded& GetOnEngineAdded()
	{
		return OnEngineAdded;
	}

	FOnEventflowEngineRemoved& GetOnEngineRemoved()
	{
		return OnEngineRemoved;
	}

	virtual void StartEventflow(const FPrimaryAssetId& AssetId) = 0;
	virtual void StopEventflow(const FPrimaryAssetId& AssetId) = 0;

protected:

	FOnEvenflowEngineAdded OnEngineAdded;
	FOnEventflowEngineRemoved OnEngineRemoved;

};

