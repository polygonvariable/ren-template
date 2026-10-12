// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestTask_EnsureGlobalTask.h"

// Project Headers
#include "System/Flow/QuestEngine.h"


void UQuestTask_EnsureGlobalTask::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UQuestTask_EnsureGlobalTask::OnLoaded(EFSMState PreviousState)
{
	UQuestEngine* Engine = GetOwningEngine<UQuestEngine>();
	checkf(IsValid(Engine), TEXT("Quest engine is invalid"));

	UQuestTask_EnsureGlobalTask* Template = GetOwningTemplate<UQuestTask_EnsureGlobalTask>();
	checkf(IsValid(Template), TEXT("Node have invalid template"));

	TArray<TInstancedStruct<FObjectiveGlobalTaskFragment>>& TemplateTasks = Template->GlobalTasks;

	for (TInstancedStruct<FObjectiveGlobalTaskFragment>& TaskFragment : TemplateTasks)
	{
		FObjectiveGlobalTaskFragment* Fragment = TaskFragment.GetMutablePtr();
		if (Fragment)
		{
			Fragment->Execute(Engine);
		}
	}

	Ready();
}

void UQuestTask_EnsureGlobalTask::OnReady(EFSMState PreviousState)
{
	Active();
}

void UQuestTask_EnsureGlobalTask::OnActive(EFSMState PreviousState)
{
	Finish(EFSMResult::Success);
}

