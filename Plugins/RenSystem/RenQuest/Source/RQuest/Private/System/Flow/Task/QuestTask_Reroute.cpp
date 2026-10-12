// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestTask_Reroute.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Core/EventflowGraphData.h"
#include "Data/QuestAsset.h"


TInstancedStruct<FEventflowNodeTransitionData>& UQuestTask_Reroute::GetTransitionData(EFSMResult Result)
{
	TInstancedStruct<FEventflowNodeTransitionData>& TransitionData = UQuestPrimaryTask::GetTransitionData(Result);
	FEventflowNodeTransitionData* Data = TransitionData.GetMutablePtr();
	checkf(Data, TEXT("Node transition data is invalid"));

	Data->NextNodeId = RerouteId;
	if (RerouteType == ERerouteType::Target)
	{
		Data->NodeTransition = EEventflowNodeTransitionType::NextNode;
	}
	else
	{
		Data->NodeTransition = EEventflowNodeTransitionType::RedirectNode;
	}
	return TransitionData;
}

void UQuestTask_Reroute::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UQuestTask_Reroute* Task = Cast<UQuestTask_Reroute>(Template);
	checkf(Task, TEXT("Invalid task template"));

	RerouteType = Task->RerouteType;
	RerouteName = Task->RerouteName;
	RerouteId = Task->RerouteId;
}

void UQuestTask_Reroute::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
#if WITH_EDITOR
	RerouteId.Invalidate();
	
	if (RerouteType == ERerouteType::Source)
	{
		const UQuestAsset* Asset = Cast<UQuestAsset>(GetOuter());
		if (IsValid(Asset))
		{
			const TMap<FGuid, FEventflowNode>& Nodes = Asset->NodeCollection;
			for (const TPair<FGuid, FEventflowNode>& Kv : Nodes)
			{
				const UQuestTask_Reroute* Reroute = Cast<UQuestTask_Reroute>(Kv.Value.Task);
				if (Reroute && Reroute->RerouteName.Equals(RerouteName) && Reroute->RerouteType == ERerouteType::Target)
				{
					RerouteId = Kv.Key;
				}
			}
		}
	}
#endif

	Super::PreSave(ObjectSaveContext);
}

void UQuestTask_Reroute::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UQuestTask_Reroute::OnLoaded(EFSMState PreviousState)
{
	Ready();
}

void UQuestTask_Reroute::OnReady(EFSMState PreviousState)
{
	Active();
}

void UQuestTask_Reroute::OnActive(EFSMState PreviousState)
{
	Finish(EFSMResult::Success);
}

