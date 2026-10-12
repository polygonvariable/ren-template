// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestPrimaryTask.h"


UQuestPrimaryTask::UQuestPrimaryTask()
{
	bAllowSubTasks = false;
}

void UQuestPrimaryTask::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UQuestPrimaryTask* Task = Cast<UQuestPrimaryTask>(Template);
	checkf(Task, TEXT("Quest task contains invalid template or doesnt match its type"));

	Summary = Task->Summary;
	bIsTransient = Task->bIsTransient;
	bCanCancel = Task->bCanCancel;
}

