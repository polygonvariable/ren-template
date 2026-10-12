// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/Task/QuestTask_SubtaskGate.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Core/QuestEvents.h"
#include "LuauLibrary.h"
#include "LuauProperty.h"
#include "LuauSubsystem.h"
#include "Task/EventflowSubTask.h"


UQuestTask_SubtaskGate::UQuestTask_SubtaskGate()
{
	bAllowSubTasks = true;
}

void UQuestTask_SubtaskGate::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
#if WITH_EDITOR
	if (bAutoCompile)
	{
		if (!FLuauLibrary::Compile(LuauCode))
		{
			LuauCode.Bytecode.Empty();
		}
	}
#endif

	Super::PreSave(ObjectSaveContext);
}

void UQuestTask_SubtaskGate::HandleOnSubTaskStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result)
{
	if (NewState != EFSMState::Finished)
	{
		return;
	}

	const UQuestTask_SubtaskGate* Template = GetOwningTemplate<UQuestTask_SubtaskGate>();
	checkf(IsValid(Template), TEXT("Invalid template task"));

	ULuauSubsystem* LuauSubsystem = ULuauSubsystem::Get(GetWorld());
	if (!IsValid(Template) || !IsValid(LuauSubsystem))
	{
		return;
	}

	FLuauProperties Inputs;
	FLuauProperties Outputs;

	FLuauProperty_Table& TaskTable = Inputs.SetTable();
	TaskTable.Set(TEXT("state"), static_cast<int>(GetState()));
	TaskTable.Set(TEXT("result"), static_cast<int>(GetResult()));

	FLuauProperty_Table& SubTaskTable = Inputs.SetTable();
	const TArray<UEventflowSubTask*>& Tasks = GetSubTasks();
	for (UEventflowSubTask* Task : Tasks)
	{
		if (IsValid(Task))
		{
			FLuauProperty_Table& SubTaskTableEntry = SubTaskTable.SetTable(Task->TaskName.ToString());
			SubTaskTableEntry.Set(TEXT("state"), static_cast<int>(Task->GetState()));
			SubTaskTableEntry.Set(TEXT("result"), static_cast<int>(Task->GetResult()));
		}
	}

	bool bSuccess = LuauSubsystem->ExecuteBytecode(Template->LuauCode.Bytecode, TEXT("script"), TEXT("evaluateSubTasks"), Inputs, Outputs);
	if (!ensureMsgf(bSuccess, TEXT("Failed to execute quest luau bytecode")))
	{
		return;
	}

	const FLuauProperty_Number* OutputResult = Outputs.Get<FLuauProperty_Number>(0);
	if (!ensureMsgf(OutputResult, TEXT("Failed to get output from luau method")))
	{
		return;
	}

	int Index = 2;
	EFSMResult FSMResult = static_cast<EFSMResult>(OutputResult->Value);
	switch (FSMResult)
	{
	case EFSMResult::Success:
		Index = 0;
		break;
	case EFSMResult::Failed:
		Index = 1;
		break;
	case EFSMResult::Cancelled:
		Index = 2;
		break;
	}

	ModifyTransitionData(
		[Index](TInstancedStruct<FEventflowNodeTransitionData>& TransitionData)
		{
			FEventflowNodeTransitionData& Data = TransitionData.GetMutable();
			Data.NextNodeIndex = Index;
		}
	);

	Finish(FSMResult);
}

void UQuestTask_SubtaskGate::OnInitialized(EFSMState PreviousState)
{
	Super::OnInitialized(PreviousState);
	Load();
}

void UQuestTask_SubtaskGate::OnLoaded(EFSMState PreviousState)
{
	Ready();
}

void UQuestTask_SubtaskGate::OnReady(EFSMState PreviousState)
{
	Active();
}

void UQuestTask_SubtaskGate::OnActive(EFSMState PreviousState)
{
	FQuestEvents::DataChanged(GetWorld());

	const TArray<UEventflowSubTask*>& Tasks = GetSubTasks();
	for (UEventflowSubTask* Task : Tasks)
	{
		if (IsValid(Task))
		{
			Task->Active();
		}
	}
}

void UQuestTask_SubtaskGate::OnFinished(EFSMResult Result)
{
	if (Result == EFSMResult::Cancelled)
	{
		ModifyTransitionData(
			[](TInstancedStruct<FEventflowNodeTransitionData>& TransitionData)
			{
				FEventflowNodeTransitionData& Data = TransitionData.GetMutable();
				Data.NextNodeIndex = 2;
			}
		);
	}
}

void UQuestTask_SubtaskGate::OnReset()
{
	Super::OnReset();
}

