// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowNode.h"

// Project Headers
#include "EventflowPin.h"


UEventflowNode* UEventflowNode::GetPreviousNodeAt(int Index) const
{
	if (!NodeInputs.IsValidIndex(Index))
	{
		return nullptr;
	}
	return GetNodeFromPinAt(NodeInputs[Index]);
}

UEventflowNode* UEventflowNode::GetNextNodeAt(int Index) const
{
	if (!NodeOutputs.IsValidIndex(Index))
	{
		return nullptr;
	}
	return GetNodeFromPinAt(NodeOutputs[Index]);
}

UEventflowNode* UEventflowNode::GetNodeFromPinAt(UEventflowPin* Pin) const
{
	if (!IsValid(Pin))
	{
		return nullptr;
	}

	UEventflowPin* LinkedPin = Pin->PinLinkedTo;
	if (!IsValid(LinkedPin))
	{
		return nullptr;
	}

	return Cast<UEventflowNode>(LinkedPin->GetOuter());
}

