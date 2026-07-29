// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/StateMachine.h"

// Generated Headers
#include "EventflowCondition.generated.h"

// Forward Declarations
class UEventflowTask;


/*
 *
 */
USTRUCT()
struct FEventflowTaskCondition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName TaskId = NAME_None;

	UPROPERTY(EditAnywhere)
	EFSMState State = EFSMState::Uninitialized;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "State == EFSMState::Finished", EditConditionHides))
	bool bUseResult = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "State == EFSMState::Finished && bUseResult", EditConditionHides))
	EFSMResult Result = EFSMResult::None;


	bool Evaluate(UEventflowTask* Task) const;

};

