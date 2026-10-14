// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestTask_GlobalTask.h"

// Project Headers
#include "System/Flow/QuestEngine.h"


void UQuestTask_EnsureGlobal::OnActive(EFSMState PreviousState)
{
	UQuestEngine* Engine = GetOwningEngine<UQuestEngine>();
	checkf(IsValid(Engine), TEXT("Quest engine is invalid"));

	UQuestTask_EnsureGlobal* Template = GetOwningTemplate<UQuestTask_EnsureGlobal>();
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

	Finish(EFSMResult::Success);
}

