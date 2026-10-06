// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/EventflowPrimaryTask.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Task/EventflowSubTask.h"
#include "Type/EventflowGraphData.h"


void UEventflowPrimaryTask::InitializeData(const FGuid& NodeId, const FEventflowNode* Node)
{
	_OwningNode = Node;
	_OwningNodeId = NodeId;
}



void UEventflowPrimaryTask::GetReturnData(TInstancedStruct<FEventflowReturnData>& ReturnData)
{
	ReturnData.Reset();

	if (TaskType == EEventflowPrimaryTaskType::Exit)
	{
		FEventflowReturnData& Data = ReturnData.InitializeAs<FEventflowReturnData>();
		Data.ExitNodeId = GetOwningNodeId();
	}
}

TInstancedStruct<FEventflowTransitionData>& UEventflowPrimaryTask::GetTransitionData(EFSMResult Result)
{
	return _TransitionData;
}

void UEventflowPrimaryTask::CreateTransitionData()
{
	_TransitionData.InitializeAs<FEventflowTransitionData>();
}

void UEventflowPrimaryTask::RemoveTransitionData()
{
	_TransitionData.Reset();
}

void UEventflowPrimaryTask::ModifyTransitionData(TFunctionRef<void(TInstancedStruct<FEventflowTransitionData>&)> TransitionData)
{
	if (_TransitionData.IsValid())
	{
		TransitionData(_TransitionData);
	}
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
		TaskTitle = Task->TaskTitle;
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




FGuid UEventflowPrimaryTask::GetOwningNodeId() const
{
	return _OwningNodeId;
}

const FEventflowNode* UEventflowPrimaryTask::GetOwningNode() const
{
	return _OwningNode;
}

const UEventflowTask* UEventflowPrimaryTask::GetOwningTemplate() const
{
	if (!_OwningNode)
	{
		return nullptr;
	}
	return _OwningNode->Task;
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
	if (!bAllowSubTasks || !_OwningNode || _ActiveSubTasks.Num() > 0)
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Current node is invalid or subtasks are already created or disabled"));
		return;
	}

	const UEventflowPrimaryTask* Template = GetOwningTemplate<UEventflowPrimaryTask>();
	if (!IsValid(Template))
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Task template is invalid"));
		return;
	}

	const TArray<UEventflowSubTask*>& Tasks = Template->SubTasks;
	for (UEventflowSubTask* Task : Tasks)
	{
		if (!IsValid(Task))
		{
			continue;
		}

		UEventflowSubTask* NewTask = NewObject<UEventflowSubTask>(this, Task->GetClass());
		if (!IsValid(NewTask))
		{
			continue;
		}

		_ActiveSubTasks.Add(NewTask);

		NewTask->OnStateChanged.BindUObject(this, &UEventflowPrimaryTask::HandleOnSubTaskStateChanged);
		NewTask->CopyFromAsset(Task);
		NewTask->Initialize();
	}
}

void UEventflowPrimaryTask::RemoveSubTasks()
{
	if (!bAllowSubTasks)
	{
		return;
	}

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

}


void UEventflowPrimaryTask::OnInitialized(EFSMState PreviousState)
{
	CreateTransitionData();
	CreateSubTasks();
}

void UEventflowPrimaryTask::OnReset()
{
	RemoveTransitionData();
	RemoveSubTasks();

	_OwningNode = nullptr;
	_OwningNodeId.Invalidate();
}

