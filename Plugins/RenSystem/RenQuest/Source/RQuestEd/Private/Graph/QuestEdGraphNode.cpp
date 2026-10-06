// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/QuestEdGraphNode.h"

// Project Headers
#include "EventflowAsset.h"
#include "Data/QuestAsset.h"
#include "Graph/EventflowEdGraphSchema.h"
#include "System/Flow/Task/QuestPrimaryTask.h"
#include "System/Flow/Task/QuestSubTask.h"



UQuestEdNode_Base::UQuestEdNode_Base()
{
	Title = FText::FromString(TEXT("Quest Node"));
}

FText UQuestEdNode_Base::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UQuestPrimaryTask* PrimaryTask = Cast<UQuestPrimaryTask>(GetTask());
	if (IsValid(PrimaryTask) && !PrimaryTask->TaskTitle.IsNone())
	{
		FString NewTitle = PrimaryTask->TaskTitle.ToString();
		const int MaxLength = 35;

		if (NewTitle.Len() > MaxLength)
		{
			NewTitle = NewTitle.Left(MaxLength) + TEXT("...");
		}

		return FText::FromString(NewTitle);
	}
	return Title;
}

bool UQuestEdNode_Base::IsEntryNode() const
{
	return false;
}









UQuestEdNode_Reroute::UQuestEdNode_Reroute()
{
	Title = FText::FromString(TEXT("Reroute"));
}

UEventflowPrimaryTask* UQuestEdNode_Reroute::GetTask() const
{
	return Task;
}

void UQuestEdNode_Reroute::SetTask(UEventflowPrimaryTask* InTask)
{
	Task = CreateOrSetTask<UQuestTask_Reroute>(InTask);
}

FLinearColor UQuestEdNode_Reroute::GetNodeTitleColor() const
{
	return FLinearColor(0.2f, 0.2f, 1.0f);
}

FText UQuestEdNode_Reroute::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UQuestTask_Reroute* PrimaryTask = Cast<UQuestTask_Reroute>(GetTask());
	if (IsValid(PrimaryTask) && !PrimaryTask->RerouteName.IsEmpty())
	{
		return FText::FromString(PrimaryTask->RerouteName);
	}
	return Super::GetNodeTitle(TitleType);
}

FText UQuestEdNode_Reroute::GetNodeDescription() const
{
	return FText::FromString(TEXT("Reroute"));
}

TArray<FText> UQuestEdNode_Reroute::GetRuntimeInputPins() const
{
	TArray<FText> RuntimePins;
	if (Task && Task->RerouteType == ERerouteType::Source)
	{
		RuntimePins.Add(FText::FromString(TEXT("in")));
	}
	return RuntimePins;
}

TArray<FText> UQuestEdNode_Reroute::GetRuntimeOutputPins() const
{
	TArray<FText> RuntimePins;
	if (Task && Task->RerouteType == ERerouteType::Target)
	{
		RuntimePins.Add(FText::FromString(TEXT("out")));
	}
	return RuntimePins;
}








UQuestEdNode_ExternalTask::UQuestEdNode_ExternalTask()
{
	Title = FText::FromString(TEXT("External Task"));
}

UEventflowPrimaryTask* UQuestEdNode_ExternalTask::GetTask() const
{
	return Task;
}

void UQuestEdNode_ExternalTask::SetTask(UEventflowPrimaryTask* InTask)
{
	Task = CreateOrSetTask<UQuestTask_ExternalTask>(InTask);
}

FLinearColor UQuestEdNode_ExternalTask::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 1.0f);
}

FText UQuestEdNode_ExternalTask::GetNodeDescription() const
{
	return FText::FromString(TEXT("External Task"));
}

void UQuestEdNode_ExternalTask::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;

	UEdGraphPin* PinOut = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("out"));
	PinOut->PinFriendlyName = FText::FromString(TEXT("out"));
	PinOut->PinType.bIsConst = true;
}

TArray<FText> UQuestEdNode_ExternalTask::GetRuntimeOutputPins() const
{
	TArray<FText> RuntimePins;
	if (Task && Task->ExternalTask)
	{
		const TArray<FName>& PinNames = Task->ExternalPinTitles;
		for (const FName& PinName : PinNames)
		{
			RuntimePins.Add(FText::FromString(PinName.ToString()));
		}
	}
	return RuntimePins;
}










UQuestEdNode_Begin::UQuestEdNode_Begin()
{
	Title = FText::FromString(TEXT("Begin"));
}

bool UQuestEdNode_Begin::IsEntryNode() const
{
	return true;
}

UEventflowPrimaryTask* UQuestEdNode_Begin::GetTask() const
{
	return Task;
}

void UQuestEdNode_Begin::SetTask(UEventflowPrimaryTask* InTask)
{
	Task = CreateOrSetTask<UQuestTask_Begin>(InTask);
}

FLinearColor UQuestEdNode_Begin::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 0.0f);
}

FText UQuestEdNode_Begin::GetNodeDescription() const
{
	return FText::FromString(TEXT("Start a quest"));
}

void UQuestEdNode_Begin::AllocateDefaultPins()
{
	UEdGraphPin* PinOut = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("out"));
	PinOut->PinFriendlyName = FText::FromString(TEXT("out"));
	PinOut->PinType.bIsConst = true;
}



UQuestEdNode_End::UQuestEdNode_End()
{
	Title = FText::FromString(TEXT("End"));
}

UEventflowPrimaryTask* UQuestEdNode_End::GetTask() const
{
	return Task;
}

void UQuestEdNode_End::SetTask(UEventflowPrimaryTask* InTask)
{
	Task = CreateOrSetTask<UQuestTask_End>(InTask);
}

FLinearColor UQuestEdNode_End::GetNodeTitleColor() const
{
	return FLinearColor(1.0f, 0.0f, 0.0f);
}

FText UQuestEdNode_End::GetNodeDescription() const
{
	return FText::FromString(TEXT("End a quest"));
}

void UQuestEdNode_End::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;
}




UQuestEdNode_SpawnMarker::UQuestEdNode_SpawnMarker()
{
	Title = FText::FromString(TEXT("Spawn Marker"));
}

UEventflowPrimaryTask* UQuestEdNode_SpawnMarker::GetTask() const
{
	return Task;
}

void UQuestEdNode_SpawnMarker::SetTask(UEventflowPrimaryTask* InTask)
{
	Task = CreateOrSetTask<UQuestTask_SpawnMarker>(InTask);
}

FLinearColor UQuestEdNode_SpawnMarker::GetNodeTitleColor() const
{
	return FLinearColor(1.0f, 0.5f, 0.0f);
}

FText UQuestEdNode_SpawnMarker::GetNodeDescription() const
{
	return FText::FromString(TEXT("Spawn a trigger zone"));
}

void UQuestEdNode_SpawnMarker::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;

	UEdGraphPin* PinOut = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("out"));
	PinOut->PinFriendlyName = FText::FromString(TEXT("out"));
	PinOut->PinType.bIsConst = true;
}

TArray<FText> UQuestEdNode_SpawnMarker::GetRuntimeOutputPins() const
{
	TArray<FText> RuntimePins;
	//if (Task)
	//{
	//	const TArray<FEventflowTransition>& Transitions = Task->TaskTransitions;
	//	for (const FEventflowTransition& Transition : Transitions)
	//	{
	//		if (Transition.Type == EEventflowTransitionType::NextNode)
	//		{
	//			const UEnum* Enum = StaticEnum<EFSMResult>();
	//			RuntimePins.Add(FText::FromString(Enum->GetNameStringByValue(static_cast<int64>(Transition.Result))));
	//		}
	//	}
	//}
	return RuntimePins;
}







UQuestEdNode_ConditionalSpawnMarker::UQuestEdNode_ConditionalSpawnMarker()
{
	Title = FText::FromString(TEXT("Conditional Spawn Marker"));
}

UEventflowPrimaryTask* UQuestEdNode_ConditionalSpawnMarker::GetTask() const
{
	return Task;
}

void UQuestEdNode_ConditionalSpawnMarker::SetTask(UEventflowPrimaryTask* InTask)
{
	Task = CreateOrSetTask<UQuestTask_ConditionalSpawnMarker>(InTask);
}

FLinearColor UQuestEdNode_ConditionalSpawnMarker::GetNodeTitleColor() const
{
	return FLinearColor(1.0f, 0.5f, 0.0f);
}

FText UQuestEdNode_ConditionalSpawnMarker::GetNodeDescription() const
{
	return FText::FromString(TEXT("Spawn a trigger zone"));
}

void UQuestEdNode_ConditionalSpawnMarker::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;

	UEdGraphPin* PinOut = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("out"));
	PinOut->PinFriendlyName = FText::FromString(TEXT("out"));
	PinOut->PinType.bIsConst = true;
}



