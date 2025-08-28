// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowNodeExternalData.h"

// Engine Headers

// Project Headers



bool UEventflowNodeTask::GetWaitForCompletion() const
{
	return bWaitForCompletion;
}

void UEventflowNodeTask::StartAction()
{
	OnActionStarted();
}

void UEventflowNodeTask::StopAction()
{
	OnActionStopped.Broadcast();
	OnActionStopped.Clear();
}

UWorld* UEventflowNodeTask::GetWorld() const
{
	return GetOuter()->GetWorld();
}

bool UEventflowNodeTask::ImplementsGetWorld() const
{
	return true;
}


