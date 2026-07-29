// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/DialogueEdGraphNode.h"

// Project Headers
#include "DialogueEngine.h"
#include "EventflowTask.h"
#include "Graph/EventflowEdGraphSchema.h"


UEventflowPrimaryTask* UDialogueEdNode_Base::GetTask() const
{
	return PrimaryTask;
}

void UDialogueEdNode_Base::SetTask(UEventflowPrimaryTask* Task)
{
	PrimaryTask = Cast<UDialogueTask>(Task);
}


FText UDialogueEdBeginNode::GetNodeDescription() const
{
	return FText::FromString(TEXT("Starts a conversation."));
}

FText UDialogueEdBeginNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Begin"));
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


FText UDialogueEdEndNode::GetNodeDescription() const
{
	return FText::FromString(TEXT("Ends a conversation."));
}

FText UDialogueEdEndNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("End"));
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


FText UDialogueEdDialogNode::GetNodeDescription() const
{
	return FText::FromString(TEXT("Conversation node."));
}

FText UDialogueEdDialogNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Dialog"));
}

FLinearColor UDialogueEdDialogNode::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 1.0f);
}

void UDialogueEdDialogNode::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("In"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("In"));
	PinIn->PinType.bIsConst = true;

	UEdGraphPin* PinOut = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("Out"));
	PinOut->PinFriendlyName = FText::FromString(TEXT("Out"));
	PinOut->PinType.bIsConst = true;
}


TArray<FText> UDialogueEdBranchNode::GetRuntimeOutputPins() const
{
	//if (!IsValid(PrimaryTask))
	//{
	//	return TArray<FText>();
	//}
	//return PrimaryTask->GetRuntimeOutputs();
	return TArray<FText>();
}

FText UDialogueEdBranchNode::GetNodeDescription() const
{
	return FText::FromString(TEXT("Conversation branch node."));
}

FText UDialogueEdBranchNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Branch"));
}

FLinearColor UDialogueEdBranchNode::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 0.5f);
}

void UDialogueEdBranchNode::AllocateDefaultPins()
{
	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("Exec"));
	Pin->PinFriendlyName = FText::FromString(TEXT("Exec"));
	Pin->PinType.bIsConst = true;
}

