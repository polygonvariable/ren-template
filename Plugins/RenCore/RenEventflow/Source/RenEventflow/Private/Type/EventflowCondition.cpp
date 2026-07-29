// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Type/EventflowCondition.h"

// Project Headers
#include "EventflowTask.h"


bool FEventflowTaskCondition::Evaluate(UEventflowTask* Task) const
{
	if (!IsValid(Task))
	{
		return false;
	}
	
	EFSMState CurrentState = Task->GetState();
	if (CurrentState == EFSMState::Finished)
	{
		if (bUseResult)
		{
			return Result == Task->GetResult();
		}
		return true;
	}

	return Task->GetState() == State;
}

