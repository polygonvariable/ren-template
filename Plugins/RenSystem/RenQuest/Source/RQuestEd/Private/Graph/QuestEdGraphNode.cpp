// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/QuestEdGraphNode.h"

// Project Headers
#include "EventflowAsset.h"
#include "Data/QuestAsset.h"
#include "Graph/EventflowEdGraphSchema.h"
#include "System/Flow/Task/QuestPrimaryTask.h"
#include "System/Flow/Task/QuestSubTask.h"



UQuestEdGraphNode::UQuestEdGraphNode()
{
	NodeTitle = FText::FromString(TEXT("Quest Node"));
}





UQuestEdNode_Reroute::UQuestEdNode_Reroute()
{
	NodeTitle = FText::FromString(TEXT("Reroute"));
}

TSubclassOf<UEventflowNodeTask> UQuestEdNode_Reroute::GetTaskClass() const
{
	return UQuestTask_Reroute::StaticClass();
}

FLinearColor UQuestEdNode_Reroute::GetNodeTitleColor() const
{
	return FLinearColor(1.0f, 0.0f, 0.5f);
}

FText UQuestEdNode_Reroute::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UQuestTask_Reroute* QuestTask = Cast<UQuestTask_Reroute>(NodeTask);

	if (IsValid(QuestTask) && !QuestTask->RerouteName.IsEmpty())
	{
		if (QuestTask->RerouteType == ERerouteType::Source)
		{
			return FText::FromString(TEXT("Goto: ") + QuestTask->RerouteName);
		}
		else
		{
			return FText::FromString(QuestTask->RerouteName);
		}
	}

	return Super::GetNodeTitle(TitleType);
}

TArray<FText> UQuestEdNode_Reroute::GetRuntimeInputPins() const
{
	TArray<FText> RuntimePins;
	UQuestTask_Reroute* QuestTask = Cast<UQuestTask_Reroute>(NodeTask);

	if (IsValid(QuestTask) && QuestTask->RerouteType == ERerouteType::Source)
	{
		RuntimePins.Add(FText::FromString(TEXT("in")));
	}

	return RuntimePins;
}

TArray<FText> UQuestEdNode_Reroute::GetRuntimeOutputPins() const
{
	TArray<FText> RuntimePins;
	UQuestTask_Reroute* QuestTask = Cast<UQuestTask_Reroute>(NodeTask);

	if (IsValid(QuestTask) && QuestTask->RerouteType == ERerouteType::Target)
	{
		RuntimePins.Add(FText::FromString(TEXT("out")));
	}
	
	return RuntimePins;
}








UQuestEdNode_ExternalTask::UQuestEdNode_ExternalTask()
{
	NodeTitle = FText::FromString(TEXT("External Task"));
}

TSubclassOf<UEventflowNodeTask> UQuestEdNode_ExternalTask::GetTaskClass() const
{
	return UQuestTask_ExternalTask::StaticClass();
}

FLinearColor UQuestEdNode_ExternalTask::GetNodeTitleColor() const
{
	return FLinearColor(0.95f, 1.0f, 0.0f);
}

void UQuestEdNode_ExternalTask::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;
}

void UQuestEdNode_ExternalTask::SyncRuntimeData()
{
	UQuestTask_ExternalTask* QuestTask = Cast<UQuestTask_ExternalTask>(NodeTask);
	if (IsValid(QuestTask))
	{
		UEventflowAsset* TaskAsset = QuestTask->ExternalAsset.LoadSynchronous();
		if (IsValid(TaskAsset))
		{
			QuestTask->ExternalPins.Empty();

			const TMap<FGuid, FEventflowNode>& Nodes = TaskAsset->NodeCollection;
			for (const TPair<FGuid, FEventflowNode>& Kv : Nodes)
			{
				UEventflowNodeTask* AssetNodeTask = Kv.Value.Task;
				if (IsValid(AssetNodeTask) && AssetNodeTask->NodeType == EEventflowNodeType::Exit)
				{
					QuestTask->ExternalPins.Add(Kv.Key);
				}
			}
		}
	}

	UQuestEdGraphNode::SyncRuntimeData();
}

TArray<FText> UQuestEdNode_ExternalTask::GetRuntimeOutputPins() const
{
	TArray<FText> RuntimePins;

	UQuestTask_ExternalTask* QuestTask = Cast<UQuestTask_ExternalTask>(NodeTask);
	if (IsValid(QuestTask))
	{
		UEventflowAsset* TaskAsset = QuestTask->ExternalAsset.LoadSynchronous();
		if (IsValid(TaskAsset))
		{
			const TMap<FGuid, FEventflowNode>& Nodes = TaskAsset->NodeCollection;
			for (const TPair<FGuid, FEventflowNode>& Kv : Nodes)
			{
				UEventflowNodeTask* AssetNodeTask = Kv.Value.Task;
				if (IsValid(AssetNodeTask) && AssetNodeTask->NodeType == EEventflowNodeType::Exit)
				{
					RuntimePins.Add(FText::FromString(AssetNodeTask->TaskName.ToString()));
				}
			}
		}
	}

	return RuntimePins;
}














UQuestEdNode_SubtaskGate::UQuestEdNode_SubtaskGate()
{
	NodeTitle = FText::FromString(TEXT("SubTask Gate"));
}

TSubclassOf<UEventflowNodeTask> UQuestEdNode_SubtaskGate::GetTaskClass() const
{
	return UQuestTask_SubtaskGate::StaticClass();
}

FLinearColor UQuestEdNode_SubtaskGate::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 0.75f, 1.0f);
}

void UQuestEdNode_SubtaskGate::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;

	UEdGraphPin* PinOutSuccess = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("success"));
	PinOutSuccess->PinFriendlyName = FText::FromString(TEXT("success"));
	PinOutSuccess->PinType.bIsConst = true;

	UEdGraphPin* PinOutFail = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("fail"));
	PinOutFail->PinFriendlyName = FText::FromString(TEXT("fail"));
	PinOutFail->PinType.bIsConst = true;

	UEdGraphPin* PinOutCancel = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("cancel"));
	PinOutCancel->PinFriendlyName = FText::FromString(TEXT("cancel"));
	PinOutCancel->PinType.bIsConst = true;
}





UQuestEdNode_WidgetGate::UQuestEdNode_WidgetGate()
{
	NodeTitle = FText::FromString(TEXT("Widget Gate"));
}

TSubclassOf<UEventflowNodeTask> UQuestEdNode_WidgetGate::GetTaskClass() const
{
	return UQuestTask_WidgetGate::StaticClass();
}

FLinearColor UQuestEdNode_WidgetGate::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 0.0f, 1.0f);
}

void UQuestEdNode_WidgetGate::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;

	UEdGraphPin* PinOutSuccess = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("success"));
	PinOutSuccess->PinFriendlyName = FText::FromString(TEXT("success"));
	PinOutSuccess->PinType.bIsConst = true;

	UEdGraphPin* PinOutFail = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("fail"));
	PinOutFail->PinFriendlyName = FText::FromString(TEXT("fail"));
	PinOutFail->PinType.bIsConst = true;

	UEdGraphPin* PinOutCancel = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("cancel"));
	PinOutCancel->PinFriendlyName = FText::FromString(TEXT("cancel"));
	PinOutCancel->PinType.bIsConst = true;
}











UQuestEdNode_EnsureGlobal::UQuestEdNode_EnsureGlobal()
{
	NodeTitle = FText::FromString(TEXT("Ensure Global"));
}

TSubclassOf<UEventflowNodeTask> UQuestEdNode_EnsureGlobal::GetTaskClass() const
{
	return UQuestTask_EnsureGlobalTask::StaticClass();
}

FLinearColor UQuestEdNode_EnsureGlobal::GetNodeTitleColor() const
{
	return FLinearColor(0.35f, 1.0f, 0.35f);
}

void UQuestEdNode_EnsureGlobal::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;

	UEdGraphPin* PinOut = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("out"));
	PinOut->PinFriendlyName = FText::FromString(TEXT("out"));
	PinOut->PinType.bIsConst = true;
}










UQuestEdNode_Begin::UQuestEdNode_Begin()
{
	NodeTitle = FText::FromString(TEXT("Begin"));
}

TSubclassOf<UEventflowNodeTask> UQuestEdNode_Begin::GetTaskClass() const
{
	return UQuestTask_Begin::StaticClass();
}

FLinearColor UQuestEdNode_Begin::GetNodeTitleColor() const
{
	return FLinearColor(0.0f, 1.0f, 0.0f);
}

void UQuestEdNode_Begin::AllocateDefaultPins()
{
	UEdGraphPin* PinOut = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("out"));
	PinOut->PinFriendlyName = FText::FromString(TEXT("out"));
	PinOut->PinType.bIsConst = true;
}



UQuestEdNode_End::UQuestEdNode_End()
{
	NodeTitle = FText::FromString(TEXT("End"));
}

TSubclassOf<UEventflowNodeTask> UQuestEdNode_End::GetTaskClass() const
{
	return UQuestTask_End::StaticClass();
}

FLinearColor UQuestEdNode_End::GetNodeTitleColor() const
{
	return FLinearColor(1.0f, 0.0f, 0.0f);
}

void UQuestEdNode_End::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;
}




UQuestEdNode_SpawnMarker::UQuestEdNode_SpawnMarker()
{
	NodeTitle = FText::FromString(TEXT("Spawn Marker"));
}

TSubclassOf<UEventflowNodeTask> UQuestEdNode_SpawnMarker::GetTaskClass() const
{
	return UQuestTask_SpawnMarker::StaticClass();
}

FLinearColor UQuestEdNode_SpawnMarker::GetNodeTitleColor() const
{
	return FLinearColor(1.0f, 0.5f, 0.0f);
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
	NodeTitle = FText::FromString(TEXT("Conditional Spawn Marker"));
}

TSubclassOf<UEventflowNodeTask> UQuestEdNode_ConditionalSpawnMarker::GetTaskClass() const
{
	return UQuestTask_ConditionalSpawnMarker::StaticClass();
}

FLinearColor UQuestEdNode_ConditionalSpawnMarker::GetNodeTitleColor() const
{
	return FLinearColor(1.0f, 0.5f, 0.0f);
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








UQuestEdNode_CheckStorage::UQuestEdNode_CheckStorage()
{
	NodeTitle = FText::FromString(TEXT("Check Storage"));
}

TSubclassOf<UEventflowNodeTask> UQuestEdNode_CheckStorage::GetTaskClass() const
{
	return UQuestTask_CheckStorage::StaticClass();
}

FLinearColor UQuestEdNode_CheckStorage::GetNodeTitleColor() const
{
	return FLinearColor(0.75f, 0.0f, 1.0f);
}

void UQuestEdNode_CheckStorage::AllocateDefaultPins()
{
	UEdGraphPin* PinIn = CreatePin(EEdGraphPinDirection::EGPD_Input, UEventflowEdGraphSchema::PC_Exec, TEXT("in"));
	PinIn->PinFriendlyName = FText::FromString(TEXT("in"));
	PinIn->PinType.bIsConst = true;

	UEdGraphPin* PinOutFound = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("found"));
	PinOutFound->PinFriendlyName = FText::FromString(TEXT("found"));
	PinOutFound->PinType.bIsConst = true;

	UEdGraphPin* PinOutNotFound = CreatePin(EEdGraphPinDirection::EGPD_Output, UEventflowEdGraphSchema::PC_Exec, TEXT("not found"));
	PinOutNotFound->PinFriendlyName = FText::FromString(TEXT("not found"));
	PinOutNotFound->PinType.bIsConst = true;
}


