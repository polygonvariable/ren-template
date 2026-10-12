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

void UQuestTask_Begin::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	FMiscLibrary::NextFrame(this, &UQuestTask_Begin::Load);
}

void UQuestTask_Begin::OnLoaded(EFSMState PreviousState)
{
	FMiscLibrary::NextFrame(this, &UQuestTask_Begin::Ready);
}

void UQuestTask_Begin::OnReady(EFSMState PreviousState)
{
	FMiscLibrary::NextFrame(this, &UQuestTask_Begin::Active);
}

void UQuestTask_Begin::OnActive(EFSMState PreviousState)
{
	Finish(EFSMResult::Success);
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

void UQuestTask_End::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	FMiscLibrary::NextFrame(this, &UQuestTask_End::Load);
}

void UQuestTask_End::OnLoaded(EFSMState PreviousState)
{
	FMiscLibrary::NextFrame(this, &UQuestTask_End::Ready);
}

void UQuestTask_End::OnReady(EFSMState PreviousState)
{
	FMiscLibrary::NextFrame(this, &UQuestTask_End::Active);
}

void UQuestTask_End::OnActive(EFSMState PreviousState)
{
	Finish(EFSMResult::Success);
}


