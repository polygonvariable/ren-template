// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowTask.h"

// Project Headers
#include "EventflowDefinition.h"
#include "EventflowEngine.h"


void UEventflowTask::InitializeTask(const FGuid& NodeId, const FEventflowNodeDefinition* Node)
{
	CurrentNodeId = NodeId;
	CurrentNode = Node;

	_bIsActive = true;

	Initialization();
}

void UEventflowTask::DeinitializeTask()
{
	Deinitialization();

	CurrentNodeId.Invalidate();
	CurrentNode = nullptr;

	_bIsActive = false;
}


void UEventflowTask::Initialization()
{
}

void UEventflowTask::Deinitialization()
{
}

void UEventflowTask::FinishTask(EEventflowDirection Direction, int Index)
{
	if (IsActive())
	{
		OnTaskFinished.ExecuteIfBound(Direction, Index);
	}
}

bool UEventflowTask::IsActive() const
{
	return _bIsActive;
}

UEventflowEngine* UEventflowTask::GetOwningEngine() const
{
	return Cast<UEventflowEngine>(GetOuter());
}



UWorld* UEventflowTask::GetWorld() const
{
	return GetOuter()->GetWorld();
}

#if WITH_EDITOR

bool UEventflowTask::ImplementsGetWorld() const
{
	return true;
}

TArray<FText> UEventflowTask::GetRuntimeInputs() const
{
	return TArray<FText>();
}

TArray<FText> UEventflowTask::GetRuntimeOutputs() const
{
	return TArray<FText>();
}

#endif

