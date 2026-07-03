// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/QuestEdGraphNode.h"

// Project Headers
#include "Data/QuestAsset.h"
#include "Graph/EventflowEdGraphSchema.h"
#include "System/Flow/Task/QuestSubTask.h"
#include "System/Flow/Task/QuestTask_TriggerZone.h"




UEventflowPrimaryTask* UQuestEdNode_Begin::GetTask() const
{
	return nullptr;
}

void UQuestEdNode_Begin::SetTask(UEventflowPrimaryTask* Task)
{

}

FText UQuestEdNode_Begin::GetNodeDescription() const
{
	return FText::FromString(TEXT("Start a quest."));
}

FText UQuestEdNode_Begin::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Begin Quest"));
}

void UQuestEdNode_Begin::AllocateDefaultPins()
{
	UEdGraphPin* PinOut = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("out"));
	PinOut->PinFriendlyName = FText::FromString(TEXT("out"));
	PinOut->PinType.bIsConst = true;
}




UEventflowPrimaryTask* UQuestEdNode_End::GetTask() const
{
	return nullptr;
}

void UQuestEdNode_End::SetTask(UEventflowPrimaryTask* Task)
{

}

FText UQuestEdNode_End::GetNodeDescription() const
{
	return FText::FromString(TEXT("End a quest."));
}

FText UQuestEdNode_End::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("End Quest"));
}

void UQuestEdNode_End::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;
}





TArray<FText> UQuestEdNode_GoToPlace::GetRuntimeOutputPins() const
{
	TArray<FText> PinNames;

	const TArray<FEventflowTransition>& Transitions = TaskTransitions;
	for (const FEventflowTransition& Transition : Transitions)
	{
		if (Transition.Type == EEventflowTransitionType::NextNode)
		{
			const UEnum* Enum = StaticEnum<EFSMResult>();
			FString ValueName = Enum->GetNameStringByValue(static_cast<int64>(Transition.Result));

			PinNames.Add(FText::FromString(ValueName));
		}
	}

	return PinNames;
}

UEventflowPrimaryTask* UQuestEdNode_GoToPlace::GetTask() const
{
	return Task;
}

void UQuestEdNode_GoToPlace::SetTask(UEventflowPrimaryTask* InTask)
{
	Task = Cast<UQuestTask_TriggerZone>(InTask);
}

TArray<UEventflowSubTask*> UQuestEdNode_GoToPlace::GetSubTasks() const
{
	TArray<UEventflowSubTask*> Tasks;
	for (UQuestSubTask* SubTask : SubTasks)
	{
		Tasks.Add(SubTask);
	}
	return Tasks;
}

void UQuestEdNode_GoToPlace::SetSubTasks(const TArray<UEventflowSubTask*>& Tasks)
{
	for (UEventflowSubTask* SubTask : Tasks)
	{
		UQuestSubTask* QuestTask = Cast<UQuestSubTask>(SubTask);
		if (IsValid(QuestTask))
		{
			SubTasks.Add(QuestTask);
		}
	}
}

bool UQuestEdNode_GoToPlace::IsEntryNode() const
{
	return true;
}

FText UQuestEdNode_GoToPlace::GetNodeDescription() const
{
	return FText::FromString(TEXT("Go to place"));
}

FText UQuestEdNode_GoToPlace::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Go to place"));
}

void UQuestEdNode_GoToPlace::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;
}




UEventflowPrimaryTask* UQuestEdNode_SubGraph::GetTask() const
{
	return nullptr;
}

void UQuestEdNode_SubGraph::SetTask(UEventflowPrimaryTask* Task)
{

}

FText UQuestEdNode_SubGraph::GetNodeDescription() const
{
	return FText::FromString(TEXT("Run subgraph."));
}

FText UQuestEdNode_SubGraph::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Subgraph"));
}

void UQuestEdNode_SubGraph::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;

	UEdGraphPin* OutSuccess = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("success"));
	OutSuccess->PinFriendlyName = FText::FromString(TEXT("success"));
	OutSuccess->PinType.bIsConst = true;
}
