// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowNodeExternalData.h"

// Engine Headers

// Project Headers



bool UEventflowNodeDataController::GetWaitForCompletion() const
{
	return bWaitForCompletion;
}

void UEventflowNodeDataController::StartTask()
{
	BP_StartTask();
}

void UEventflowNodeDataController::StopTask()
{
	OnTaskComplete.Broadcast();
	OnTaskComplete.Clear();
}

UWorld* UEventflowNodeDataController::BP_GetWorld() const
{
	return GetWorld();
}

UWorld* UEventflowNodeDataController::GetWorld() const
{
	return GetOuter()->GetWorld();
}


