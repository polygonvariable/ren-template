// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/QuestEdGraphNode.h"

// Engine Headers

// Project Headers
#include "RenQuest/Public/Quest/QuestAsset.h"
#include "RenQuest/Public/Quest/QuestNodeData.h"

#include "RenEventflowEd/Public/Graph/EventflowEdGraphSchema.h"



FName UQuestEdBeginNode::GetNodeType() const
{
	return FName(TEXT("REN.EF.QUEST.BEGIN"));
}

FText UQuestEdBeginNode::GetNodeDescription() const
{
	return FText::FromString(TEXT("Starts a new quest"));
}

FText UQuestEdBeginNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Begin Quest"));
}

TSubclassOf<UEventflowNodeData> UQuestEdBeginNode::GetNodeDataClass() const
{
	return UEventflowNodeData::StaticClass();
}

FLinearColor UQuestEdBeginNode::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 0.25f);
}

void UQuestEdBeginNode::AllocateDefaultPins()
{
	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("ExecOut"));
	Pin->PinFriendlyName = FText::FromString(TEXT("out"));
	Pin->PinType.bIsConst = true;
}

bool UQuestEdBeginNode::IsEntryNode() const
{
	return true;
}



FName UQuestEdObjectiveNode::GetNodeType() const
{
	return FName(TEXT("REN.EF.QUEST.OBJECTIVE"));
}

FText UQuestEdObjectiveNode::GetNodeDescription() const
{
	return FText::FromString(TEXT("Quest objective"));
}

FText UQuestEdObjectiveNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UQuestObjectiveNodeData* NodeData = Cast<UQuestObjectiveNodeData>(GetNodeData());
	if (!NodeData)
	{
		return FText::FromString(TEXT("Quest Objective"));
	}

	FString NodeContent = LimitTextLength(NodeData->ObjectiveTitle.ToString(), 15);
	return FText::FromString(TEXT("Objective: ") + NodeContent);
}

TSubclassOf<UEventflowNodeData> UQuestEdObjectiveNode::GetNodeDataClass() const
{
	return UQuestObjectiveNodeData::StaticClass();
}

FLinearColor UQuestEdObjectiveNode::GetNodeTitleColor() const
{
	return FColor::Orange;
}

void UQuestEdObjectiveNode::AllocateDefaultPins()
{
	UEdGraphPin* InPin = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("ExecIn"));
	InPin->PinFriendlyName = FText::FromString(TEXT("in"));
	InPin->PinType.bIsConst = true;

	UEdGraphPin* SuccessPin = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("ExecSuccess"));
	SuccessPin->PinFriendlyName = FText::FromString(TEXT("next"));
	SuccessPin->PinType.bIsConst = true;

	UEdGraphPin* FailPin = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("ExecFail"));
	FailPin->PinFriendlyName = FText::FromString(TEXT("fail"));
	FailPin->PinType.bIsConst = true;
}

bool UQuestEdObjectiveNode::CanCreateRuntimeOutputPins() const
{
	return true;
}



FName UQuestEdEndNode::GetNodeType() const
{
	return FName(TEXT("REN.EF.QUEST.END"));
}

FText UQuestEdEndNode::GetNodeDescription() const
{
	return FText::FromString(TEXT("End current quest"));
}

FText UQuestEdEndNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("End Quest"));
}

TSubclassOf<UEventflowNodeData> UQuestEdEndNode::GetNodeDataClass() const
{
	return UEventflowNodeData::StaticClass();
}

FLinearColor UQuestEdEndNode::GetNodeTitleColor() const
{
	return FLinearColor(1.0f, 0.0f, 0.0f);
}

void UQuestEdEndNode::AllocateDefaultPins()
{
	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("ExecIn"));
	Pin->PinFriendlyName = FText::FromString(TEXT("in"));
	Pin->PinType.bIsConst = true;
}

