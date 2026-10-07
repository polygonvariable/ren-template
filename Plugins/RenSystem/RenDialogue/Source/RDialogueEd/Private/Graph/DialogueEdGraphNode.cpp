// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/DialogueEdGraphNode.h"

// Project Headers
#include "DialogueEngine.h"
#include "DialogueTask.h"
#include "EventflowTask.h"
#include "Graph/EventflowEdGraphSchema.h"



UDialogueEdNode_Base::UDialogueEdNode_Base()
{

}





UDialogueEdNode_Begin::UDialogueEdNode_Begin()
{
	NodeTitle = FText::FromString(TEXT("Begin"));
}

TSubclassOf<UEventflowNodeTask> UDialogueEdNode_Begin::GetTaskClass() const
{
	return UDialogueTask_Begin::StaticClass();
}

FLinearColor UDialogueEdNode_Begin::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 0.25f);
}

void UDialogueEdNode_Begin::AllocateDefaultPins()
{
	UEdGraphPin* PinOut = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("out"));
	PinOut->PinFriendlyName = FText::FromString(TEXT("out"));
	PinOut->PinType.bIsConst = true;
}







UDialogueEdNode_End::UDialogueEdNode_End()
{
	NodeTitle = FText::FromString(TEXT("End"));
}

TSubclassOf<UEventflowNodeTask> UDialogueEdNode_End::GetTaskClass() const
{
	return UDialogueTask_End::StaticClass();
}

FLinearColor UDialogueEdNode_End::GetNodeTitleColor() const
{
	return FLinearColor(1.0f, 0.0f, 0.0f);
}

void UDialogueEdNode_End::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;
}






UDialogueEdNode_Dialogue::UDialogueEdNode_Dialogue()
{
	NodeTitle = FText::FromString(TEXT("Dialogue"));
}

TSubclassOf<UEventflowNodeTask> UDialogueEdNode_Dialogue::GetTaskClass() const
{
	return UDialogueTask_Dialogue::StaticClass();
}

FLinearColor UDialogueEdNode_Dialogue::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 1.0f);
}

void UDialogueEdNode_Dialogue::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;

	UEdGraphPin* PinOut = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("out"));
	PinOut->PinFriendlyName = FText::FromString(TEXT("out"));
	PinOut->PinType.bIsConst = true;
}





UDialogueEdNode_Branch::UDialogueEdNode_Branch()
{
	NodeTitle = FText::FromString(TEXT("Branch"));
}

TSubclassOf<UEventflowNodeTask> UDialogueEdNode_Branch::GetTaskClass() const
{
	return UDialogueTask_Branch::StaticClass();
}

TArray<FText> UDialogueEdNode_Branch::GetRuntimeOutputPins() const
{
	UDialogueTask_Branch* DialogueTask = Cast<UDialogueTask_Branch>(NodeTask);
	if (!IsValid(DialogueTask))
	{
		return TArray<FText>();
	}
	return DialogueTask->Options;
}

FLinearColor UDialogueEdNode_Branch::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 0.5f);
}

void UDialogueEdNode_Branch::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;
}

