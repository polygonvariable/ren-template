// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/DialogueEdGraphNode.h"

// Engine Headers

// Project Headers
#include "RenEventflowEditor/Public/Graph/EventflowEdGraphSchema.h"



FLinearColor UDialogueEdBeginNode::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 0.25f);
}

void UDialogueEdBeginNode::AllocateDefaultPins()
{
	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("Exec"));
	Pin->PinFriendlyName = FText::FromString(TEXT("Exec"));
	Pin->PinType.bIsConst = true;
}

bool UDialogueEdBeginNode::IsEntryNode() const
{
	return true;
}

bool UDialogueEdBeginNode::CanCreateRuntimeOutputPins() const
{
	return false;
}



FLinearColor UDialogueEdEndNode::GetNodeTitleColor() const
{
	return FLinearColor(1.0f, 0.0f, 0.0f);
}

void UDialogueEdEndNode::AllocateDefaultPins()
{
	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("Exec"));
	Pin->PinFriendlyName = FText::FromString(TEXT("Exec"));
	Pin->PinType.bIsConst = true;
}

bool UDialogueEdEndNode::CanCreateRuntimeOutputPins() const
{
	return false;
}



FLinearColor UDialogueEdDialogNode::GetNodeTitleColor() const
{
	return FLinearColor(1.0f, 1.0f, 1.0f);
}

void UDialogueEdDialogNode::AllocateDefaultPins()
{
	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("Exec"));
	Pin->PinFriendlyName = FText::FromString(TEXT("Exec"));
	Pin->PinType.bIsConst = true;
}

bool UDialogueEdDialogNode::CanCreateRuntimeOutputPins() const
{
	return true;
}
