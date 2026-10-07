// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Task/EventflowNodeTask.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Task/EventflowSubTask.h"
#include "Type/EventflowGraphData.h"


void UEventflowNodeTask::InitializeData(const FGuid& NodeId, const FEventflowNode* Node)
{
	_OwningNode = Node;
	_OwningNodeId = NodeId;
}


void UEventflowNodeTask::GetReturnData(TInstancedStruct<FEventflowReturnData>& ReturnData)
{
	ReturnData.Reset();

	if (NodeType == EEventflowNodeType::Exit)
	{
		FEventflowReturnData& Data = ReturnData.InitializeAs<FEventflowReturnData>();
		Data.ExitNodeId = GetOwningNodeId();
	}
}

TInstancedStruct<FEventflowNodeTransitionData>& UEventflowNodeTask::GetTransitionData(EFSMResult Result)
{
	return _TransitionData;
}

void UEventflowNodeTask::ModifyTransitionData(TFunctionRef<void(TInstancedStruct<FEventflowNodeTransitionData>&)> TransitionData)
{
	if (_TransitionData.IsValid())
	{
		TransitionData(_TransitionData);
	}
}


const TArray<TObjectPtr<UEventflowSubTask>>& UEventflowNodeTask::GetSubTasks()
{
	return _ActiveSubTasks;
}

UEventflowSubTask* UEventflowNodeTask::GetSubTask(const FName& TaskName) const
{
	const TObjectPtr<UEventflowSubTask>* FoundTask = _ActiveSubTasks.FindByPredicate([TaskName](UEventflowSubTask* Task) { return Task->TaskName == TaskName; });
	if (!FoundTask)
	{
		return nullptr;
	}
	return FoundTask->Get();
}


void UEventflowNodeTask::CopyFromAsset(const UEventflowTask* Template)
{
	const UEventflowNodeTask* TaskTemplate = Cast<UEventflowNodeTask>(Template);
	check(IsValid(TaskTemplate));

	NodeTitle = TaskTemplate->NodeTitle;
}

#if WITH_EDITOR
void UEventflowNodeTask::AppendAssetBundleData(FAssetBundleData& AssetBundle)
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
EDataValidationResult UEventflowNodeTask::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (bAllowSubTasks)
	{
		for (UEventflowSubTask* SubTask : SubTasks)
		{
			if (!IsValid(SubTask))
			{
				Context.AddError(FText::FromString("Invalid sub task instance in task"));
				return EDataValidationResult::Invalid;
			}

			EDataValidationResult TaskResult = SubTask->IsDataValid(Context);
			if (TaskResult == EDataValidationResult::Invalid)
			{
				return EDataValidationResult::Invalid;
			}
		}
	}

	return Result;
}
#endif


void UEventflowNodeTask::CreateTransitionData()
{
	_TransitionData.InitializeAs<FEventflowNodeTransitionData>();
}

void UEventflowNodeTask::RemoveTransitionData()
{
	_TransitionData.Reset();
}


FGuid UEventflowNodeTask::GetOwningNodeId() const
{
	return _OwningNodeId;
}

const FEventflowNode* UEventflowNodeTask::GetOwningNode() const
{
	return _OwningNode;
}

UEventflowTask* UEventflowNodeTask::GetOwningTemplate() const
{
	if (!_OwningNode)
	{
		return nullptr;
	}
	return _OwningNode->Task;
}


void UEventflowNodeTask::CreateSubTasks()
{
	if (!bAllowSubTasks || _ActiveSubTasks.Num() > 0)
	{
		LOG_WARNING(LogEventflowEngine, TEXT("Current node is invalid or subtasks are already created or disabled"));
		return;
	}

	const UEventflowNodeTask* Template = GetOwningTemplate<UEventflowNodeTask>();
	check(IsValid(Template));

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

		NewTask->OnStateChanged.BindUObject(this, &UEventflowNodeTask::HandleOnSubTaskStateChanged);
		NewTask->CopyFromAsset(Task);
		NewTask->Initialize();
	}
}

void UEventflowNodeTask::RemoveSubTasks()
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


void UEventflowNodeTask::HandleOnSubTaskStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result)
{

}

void UEventflowNodeTask::OnInitialized(EFSMState PreviousState)
{
	CreateTransitionData();
	CreateSubTasks();
}

void UEventflowNodeTask::OnReset()
{
	RemoveTransitionData();
	RemoveSubTasks();

	_OwningNode = nullptr;
	_OwningNodeId.Invalidate();
}

