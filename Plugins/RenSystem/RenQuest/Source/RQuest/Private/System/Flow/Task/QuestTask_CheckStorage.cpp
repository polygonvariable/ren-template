// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestTask_CheckStorage.h"


void UQuestTask_CheckStorage::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UQuestTask_CheckStorage::OnLoaded(EFSMState PreviousState)
{
	Ready();
}

void UQuestTask_CheckStorage::OnReady(EFSMState PreviousState)
{
	Active();
}

void UQuestTask_CheckStorage::OnActive(EFSMState PreviousState)
{
	ModifyTransitionData(
		[](TInstancedStruct<FEventflowNodeTransitionData>& TransitionData)
		{
			FEventflowNodeTransitionData& Data = TransitionData.GetMutable();
			Data.NextNodeIndex = 1;
		}
	);
	Finish(EFSMResult::Success);
}

