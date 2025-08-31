// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowNodeTask.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowNode.h"



void UEventflowNodeTask::StartAction()
{
	OnActionStarted();
}

void UEventflowNodeTask::StopAction(bool bSuccess)
{
	OnActionStopped.ExecuteIfBound(bSuccess);
	OnActionStopped.Unbind();
}

void UEventflowNodeTask::OnActionStarted_Implementation()
{
	StopAction(true);
}

void UEventflowNodeTask::SetOwnedNode(UEventflowNode* Node)
{
	OwnedNode = Node;
}

void UEventflowNodeTask::SetOwnedGraphAsset(UEventflowAsset* Asset)
{
	OwnedGraphAsset = Asset;
}

void UEventflowNodeTask::ResetData()
{
	OnActionStopped.Unbind();
	OwnedNode.Reset();
	OwnedGraphAsset.Reset();
}

UWorld* UEventflowNodeTask::GetWorld() const
{
	return GetOuter()->GetWorld();
}

#if WITH_EDITOR

bool UEventflowNodeTask::ImplementsGetWorld() const
{
	return true;
}

#endif

