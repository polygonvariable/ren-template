// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers
#include "StateMachine/FiniteStateMachineType.h"

// Generated Headers
#include "IObjectiveFeedback.generated.h"

// Delegate Declarations
DECLARE_DELEGATE_OneParam(FOnInteractionCompleted, EFSMResult);


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UObjectiveFeedback : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class IObjectiveFeedback
{

	GENERATED_BODY()

public:

	FOnInteractionCompleted& GetOnObjectiveFeedback()
	{
		return OnInteractionCompleted;
	};

protected:

	FOnInteractionCompleted OnInteractionCompleted;

};

