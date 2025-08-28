// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/StoryboardEdGraphNode.h"

// Engine Headers

// Project Headers
#include "RenQuest/Public/StoryboardAsset.h"

#include "RenEventflowEd/Public/Graph/EventflowEdGraphSchema.h"



FName UStoryboardEdBeginNode::GetNodeType() const
{
	return FName(TEXT("REN.EF.STORY.BEGIN"));
}

FText UStoryboardEdBeginNode::GetNodeDescription() const
{
	return FText::FromString(TEXT("Starts a new quests story"));
}

FText UStoryboardEdBeginNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Begin Story"));
}

TSubclassOf<UEventflowNodeData> UStoryboardEdBeginNode::GetNodeDataClass() const
{
	return UEventflowNodeData::StaticClass();
}

FLinearColor UStoryboardEdBeginNode::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 0.25f);
}

void UStoryboardEdBeginNode::AllocateDefaultPins()
{
	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("ExecOut"));
	Pin->PinFriendlyName = FText::FromString(TEXT("Exec"));
	Pin->PinType.bIsConst = true;
}

bool UStoryboardEdBeginNode::IsEntryNode() const
{
	return true;
}



FName UStoryboardEdQuestNode::GetNodeType() const
{
	return FName(TEXT("REN.EF.STORY.QUEST"));
}

FText UStoryboardEdQuestNode::GetNodeDescription() const
{
	return FText::FromString(TEXT("Quest node"));
}

FText UStoryboardEdQuestNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UStoryboardQuestData* NodeData = Cast<UStoryboardQuestData>(GetNodeData());
	if (!NodeData)
	{
		return FText::FromString(TEXT("Quest"));
	}

	FString NodeContent = LimitTextLength(NodeData->QuestTitle.ToString(), 15);
	return FText::FromString(TEXT("Quest: ") + NodeContent);
}

TSubclassOf<UEventflowNodeData> UStoryboardEdQuestNode::GetNodeDataClass() const
{
	return UStoryboardQuestData::StaticClass();
}

FLinearColor UStoryboardEdQuestNode::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 1.0f);
}

void UStoryboardEdQuestNode::AllocateDefaultPins()
{
	UEdGraphPin* InPin = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("ExecIn"));
	InPin->PinFriendlyName = FText::FromString(TEXT("Exec"));
	InPin->PinType.bIsConst = true;

	UEdGraphPin* OutPin = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("ExecOut"));
	OutPin->PinFriendlyName = FText::FromString(TEXT("Exec"));
	OutPin->PinType.bIsConst = true;
}

bool UStoryboardEdQuestNode::CanCreateRuntimeOutputPins() const
{
	return true;
}

