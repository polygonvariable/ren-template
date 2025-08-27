// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowNode.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowPin.h"



UEventflowNode* UEventflowNode::GetNextNodeAt(int Index) const
{
	if (!OutputPins.IsValidIndex(Index))
	{
		return nullptr;
	}

	UEventflowPin* Pin = OutputPins[Index];
	if (!Pin)
	{
		return nullptr;
	}

	UEventflowPin* LinkedPin = Pin->PinLinkedTo;
	if (!LinkedPin)
	{
		return nullptr;
	}

	return Cast<UEventflowNode>(LinkedPin->GetOuter());
}

