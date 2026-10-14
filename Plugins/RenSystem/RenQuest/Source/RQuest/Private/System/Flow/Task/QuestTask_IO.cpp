// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestTask_IO.h"

// Project Headers
#include "Core/EventflowGraphData.h"
#include "Core/EventflowIO.h"
#include "MiscLibrary.h"


UQuestTask_Begin::UQuestTask_Begin()
{
	NodeType = EEventflowNodeType::Entry;
}


UQuestTask_End::UQuestTask_End()
{
	NodeType = EEventflowNodeType::Exit;
}

void UQuestTask_End::GetReturnData(TInstancedStruct<FEventflowReturnData>& ReturnData)
{
	UQuestPrimaryTask::GetReturnData(ReturnData);

	FEventflowReturnData* Data = ReturnData.GetMutablePtr();
	checkf(Data, TEXT("Node return data is invalid"));

	Data->GraphTransition = GraphResult;
}

void UQuestTask_End::CopyFromAsset(const UEventflowTask* Template)
{
	Super::CopyFromAsset(Template);

	const UQuestTask_End* Task = Cast<UQuestTask_End>(Template);
	checkf(IsValid(Task), TEXT("Node have invalid task template"));

	GraphResult = Task->GraphResult;
}

