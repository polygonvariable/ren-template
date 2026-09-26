// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/EventflowPrimaryTask.h"

// Project Headers
#include "Type/EventflowGraphData.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Task/EventflowSubTask.h"


void UEventflowPrimaryTask::InitializeData(const FGuid& NodeId, const FEventflowNode* Node)
{
	_CurrentNode = Node;
	_CurrentNodeId = NodeId;
}


int UEventflowPrimaryTask::GetTransitionIndex(EFSMResult Result) const
{
	return _TransitionIndex;
}

EEventflowTransitionType UEventflowPrimaryTask::GetTransitionType(EFSMResult Result) const
{
	const FEventflowTransition* FoundTransition = TaskTransitions.FindByPredicate([Result](const FEventflowTransition& Transition) { return Transition.Result == Result; });
	if (FoundTransition)
	{
		return FoundTransition->Type;
	}
	return EEventflowTransitionType::NextNode;
}

const TArray<TObjectPtr<UEventflowSubTask>>& UEventflowPrimaryTask::GetSubTasks()
{
	return _ActiveSubTasks;
}


void UEventflowPrimaryTask::CopyFromAsset(const UEventflowTask* Template)
{
	const UEventflowPrimaryTask* Task = Cast<UEventflowPrimaryTask>(Template);
	if (IsValid(Task))
	{
		TaskTransitions = Task->TaskTransitions;
		SubTaskConditions = Task->SubTaskConditions;
	}
}


#if WITH_EDITOR

void UEventflowPrimaryTask::AppendAssetBundleData(FAssetBundleData& AssetBundle)
{
	Super::AppendAssetBundleData(AssetBundle);

	for (TObjectPtr<UEventflowSubTask> Task : SubTasks)
	{
		if (IsValid(Task))
		{
			Task->AppendAssetBundleData(AssetBundle);
		}
	}
}

#endif


void UEventflowPrimaryTask::SetTransitionIndex(int Index)
{
	_TransitionIndex = Index;
}


UEventflowSubTask* UEventflowPrimaryTask::GetSubTask(const FName& TaskName) const
{
	const TObjectPtr<UEventflowSubTask>* FoundTask = _ActiveSubTasks.FindByPredicate([TaskName](UEventflowSubTask* Task) { return Task->TaskName == TaskName; });
	if (!FoundTask)
	{
		return nullptr;
	}
	return FoundTask->Get();
}

void UEventflowPrimaryTask::CreateSubTasks()
{
	if (!_CurrentNode || _ActiveSubTasks.Num() > 0)
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Current node is invalid or subtasks are already created"));
		return;
	}

	const UEventflowPrimaryTask* TaskTemplate = Cast<UEventflowPrimaryTask>(_CurrentNode->Task);
	if (!IsValid(TaskTemplate))
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Task template is invalid"));
		return;
	}

	const TArray<UEventflowSubTask*>& Tasks = TaskTemplate->SubTasks;
	for (UEventflowSubTask* Task : Tasks)
	{
		if (!IsValid(Task))
		{
			continue;
		}

		UClass* Class = Task->GetClass();

		UEventflowSubTask* NewTask = NewObject<UEventflowSubTask>(this, Class);
		NewTask->OnStateChanged.BindUObject(this, &UEventflowPrimaryTask::HandleOnSubTaskStateChanged);
		NewTask->CopyFromAsset(Task);
		NewTask->Initialize();

		_ActiveSubTasks.Add(NewTask);
	}
}

void UEventflowPrimaryTask::RemoveSubTasks()
{
	for (UEventflowSubTask* Task : _ActiveSubTasks)
	{
		if (!IsValid(Task))
		{
			continue;
		}

		Task->OnStateChanged.Unbind();

		if (Task->GetState() == EFSMState::Active)
		{
			Task->Finish(EFSMResult::Aborted);
		}
		if (Task->GetState() != EFSMState::Uninitialized)
		{
			Task->Reset();
		}

		Task->MarkAsGarbage();
	}
	_ActiveSubTasks.Empty();
}


void UEventflowPrimaryTask::HandleOnSubTaskStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result)
{
	if (PreviousState == EFSMState::Active && NewState == EFSMState::Finished)
	{
		Finish(Result);
	}
}


void UEventflowPrimaryTask::OnInitialized(EFSMState PreviousState)
{
	CreateSubTasks();
}

void UEventflowPrimaryTask::OnReset()
{
	RemoveSubTasks();

	_CurrentNode = nullptr;
	_CurrentNodeId.Invalidate();
}

