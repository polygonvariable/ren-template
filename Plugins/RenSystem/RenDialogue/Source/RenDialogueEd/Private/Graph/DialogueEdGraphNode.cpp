// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/DialogueEdGraphNode.h"

// Engine Headers

// Project Headers
#include "RenDialogue/Public/DialogueAsset.h"

#include "RenEventflowEd/Public/Graph/EventflowEdGraphSchema.h"



FName UDialogueEdBeginNode::GetNodeType() const
{
	return FName(TEXT("REN.EF.DIA.BEGIN"));
}

FText UDialogueEdBeginNode::GetNodeDescription() const
{
	return FText::FromString(TEXT("Starts a conversation."));
}

FText UDialogueEdBeginNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UDialogueNodeData* NodeData = Cast<UDialogueNodeData>(GetNodeData());
	if (!NodeData)
	{
		return FText::FromString(TEXT("Begin"));
	}

	FString NodeContent = LimitTextLength(NodeData->DialogueTitle.ToString(), 15);
	return FText::FromString(TEXT("Dialogue: ") + NodeContent);
}

TSubclassOf<UEventflowNodeData> UDialogueEdBeginNode::GetNodeDataClass() const
{
	return UDialogueNodeData::StaticClass();
}

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



FName UDialogueEdEndNode::GetNodeType() const
{
	return FName(TEXT("REN.EF.DIA.END"));
}

FText UDialogueEdEndNode::GetNodeDescription() const
{
	return FText::FromString(TEXT("Ends a conversation."));
}

FText UDialogueEdEndNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("End"));
}

TSubclassOf<UEventflowNodeData> UDialogueEdEndNode::GetNodeDataClass() const
{
	return UDialogueNodeData::StaticClass();
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



FName UDialogueEdDialogNode::GetNodeType() const
{
	return FName(TEXT("REN.EF.DIA.DIALOG"));
}

FText UDialogueEdDialogNode::GetNodeDescription() const
{
	return FText::FromString(TEXT("Conversation node."));
}

FText UDialogueEdDialogNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UDialogueNodeData* NodeData = Cast<UDialogueNodeData>(GetNodeData());
	if (!NodeData)
	{
		return FText::FromString(TEXT("Dialogue"));
	}
	
	FString NodeContent = LimitTextLength(NodeData->DialogueContent.ToString(), 15);
	return FText::FromString(TEXT("Dialogue: ") + NodeContent);
}

TSubclassOf<UEventflowNodeData> UDialogueEdDialogNode::GetNodeDataClass() const
{
	return UDialogueNodeData::StaticClass();
}

FLinearColor UDialogueEdDialogNode::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 1.0f);
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
