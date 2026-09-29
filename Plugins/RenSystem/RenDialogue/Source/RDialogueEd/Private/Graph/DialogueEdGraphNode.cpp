// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/DialogueEdGraphNode.h"

// Project Headers
#include "DialogueEngine.h"
#include "DialogueTask.h"
#include "EventflowTask.h"
#include "Graph/EventflowEdGraphSchema.h"


UEventflowPrimaryTask* UDialogueEdNode_Base::GetTask() const
{
	return Task;
}

void UDialogueEdNode_Base::SetTask(UEventflowPrimaryTask* InTask)
{
	Task = Cast<UDialogueTask_Default>(InTask);
}



FText UDialogueEdNode_Begin::GetNodeDescription() const
{
	return FText::FromString(TEXT("Starts a conversation."));
}

FText UDialogueEdNode_Begin::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Begin"));
}

FLinearColor UDialogueEdNode_Begin::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 0.25f);
}

void UDialogueEdNode_Begin::AllocateDefaultPins()
{
	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("Exec"));
	Pin->PinFriendlyName = FText::FromString(TEXT("Exec"));
	Pin->PinType.bIsConst = true;
}



FText UDialogueEdNode_End::GetNodeDescription() const
{
	return FText::FromString(TEXT("Ends a conversation."));
}

FText UDialogueEdNode_End::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("End"));
}

FLinearColor UDialogueEdNode_End::GetNodeTitleColor() const
{
	return FLinearColor(1.0f, 0.0f, 0.0f);
}

void UDialogueEdNode_End::AllocateDefaultPins()
{
	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("Exec"));
	Pin->PinFriendlyName = FText::FromString(TEXT("Exec"));
	Pin->PinType.bIsConst = true;
}



FText UDialogueEdNode_Dialogue::GetNodeDescription() const
{
	return FText::FromString(TEXT("Conversation node"));
}

FText UDialogueEdNode_Dialogue::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Dialog"));
}

FLinearColor UDialogueEdNode_Dialogue::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 1.0f);
}

void UDialogueEdNode_Dialogue::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("In"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("In"));
	PinIn->PinType.bIsConst = true;

	UEdGraphPin* PinOut = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("Out"));
	PinOut->PinFriendlyName = FText::FromString(TEXT("Out"));
	PinOut->PinType.bIsConst = true;
}



UEventflowPrimaryTask* UDialogueEdNode_Branch::GetTask() const
{
	return Task;
}

void UDialogueEdNode_Branch::SetTask(UEventflowPrimaryTask* InTask)
{
	Task = Cast<UDialogueTask_Branch>(InTask);
}

TArray<FText> UDialogueEdNode_Branch::GetRuntimeOutputPins() const
{
	UDialogueTask_Branch* DialogueTask = Cast<UDialogueTask_Branch>(Task);
	if (!IsValid(DialogueTask))
	{
		return TArray<FText>();
	}
	return DialogueTask->Options;
}

FText UDialogueEdNode_Branch::GetNodeDescription() const
{
	return FText::FromString(TEXT("Conversation branch node"));
}

FText UDialogueEdNode_Branch::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Branch"));
}

FLinearColor UDialogueEdNode_Branch::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 0.5f);
}

void UDialogueEdNode_Branch::AllocateDefaultPins()
{
	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("Exec"));
	Pin->PinFriendlyName = FText::FromString(TEXT("Exec"));
	Pin->PinType.bIsConst = true;
}

