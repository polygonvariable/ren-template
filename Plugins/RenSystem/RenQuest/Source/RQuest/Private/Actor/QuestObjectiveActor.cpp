// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/QuestObjectiveActor.h"

// Project Headers
#include "Task/EventflowTask.h"


void AQuestObjectiveActor::SetOwningTask(UEventflowTask* Task)
{
	OwningTask = Task;
}

void AQuestObjectiveActor::StartTasks()
{
	UEventflowTask* Task = OwningTask.Get();
	if (IsValid(Task))
	{
		Task->Execute();
	}
}

void AQuestObjectiveActor::AbortTasks()
{
	UEventflowTask* Task = OwningTask.Get();
	if (IsValid(Task))
	{
		Task->Finish(EFSMResult::Aborted);
	}
}

void AQuestObjectiveActor::CompleteTasks()
{
	// for debug only, as actor should not directly complete the primary task
	UEventflowTask* Task = OwningTask.Get();
	if (IsValid(Task))
	{
		Task->Finish(EFSMResult::Success);
	}
}

void AQuestObjectiveActor::CompleteObjective(bool bSuccess)
{
	OnCompleted.ExecuteIfBound();
}

