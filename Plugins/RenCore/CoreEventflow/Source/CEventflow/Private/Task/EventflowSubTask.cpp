// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/EventflowSubTask.h"


void UEventflowSubTask::CopyFromAsset(const UEventflowTask* Template)
{
	const UEventflowSubTask* Task = Cast<UEventflowSubTask>(Template);
	if (IsValid(Task))
	{
		TaskName = Task->TaskName;
	}
}

