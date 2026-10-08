// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEngine.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Core/AssetManagerLibrary.h"
#include "Core/PoolLibrary.h"
#include "EventflowAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Task/EventflowNodeTask.h"
#include "Task/EventflowGlobalTask.h"


void UEventflowEngine::InitializeData(const FPrimaryAssetId& InAssetId, const FEventflowEntryData& InEntryData)
{
	_AssetId = InAssetId;
	_EntryData = InEntryData;
}


UWorld* UEventflowEngine::GetWorld() const
{
	return GetOuter()->GetWorld();
}

UEventflowNodeTask* UEventflowEngine::GetNodeTask() const
{
	return _ActiveNodeTask.Get();
}

UEventflowAsset* UEventflowEngine::GetAsset() const
{
	return _Asset;
}

const TInstancedStruct<FEventflowReturnData>& UEventflowEngine::GetReturnData() const
{
	return _ReturnData;
}


UEventflowGlobalTask* UEventflowEngine::EnsureGlobalTask(FName TaskName)
{
	if (!ensureAlwaysMsgf(TaskName.IsValid(), TEXT("Global task name is invalid")))
	{
		return nullptr;
	}

	TObjectPtr<UEventflowGlobalTask>* FoundActiveTask = _ActiveGlobalTasks.FindByPredicate([TaskName](UEventflowGlobalTask* Task) { return IsValid(Task) && Task->TaskName == TaskName; });
	if (FoundActiveTask)
	{
		return FoundActiveTask->Get();
	}

	UEventflowAsset* Asset = GetAsset();
	checkf(IsValid(Asset), TEXT("Eventflow enigne have invalid asset"));

	UEventflowGlobalTask* GlobalTask = Asset->GetGlobalTask(TaskName);
	if (!ensureAlwaysMsgf(IsValid(GlobalTask), TEXT("Eventflow asset have invalid global task instance")))
	{
		return nullptr;
	}

	UEventflowGlobalTask* NewTask = NewObject<UEventflowGlobalTask>(this, GlobalTask->GetClass());
	if (!ensureAlwaysMsgf(IsValid(NewTask), TEXT("Failed to create new global task")))
	{
		return nullptr;
	}

	_ActiveGlobalTasks.Add(NewTask);

	NewTask->CopyFromAsset(GlobalTask);
	NewTask->Initialize();

	return NewTask;
}


void UEventflowEngine::GetAssetBundle(TArray<FName>& OutBundle) const
{
}

const FEventflowNode* UEventflowEngine::GetNode(const FGuid& NodeId) const
{
	UEventflowAsset* Asset = GetAsset();
	checkf(IsValid(Asset), TEXT("Eventflow enigne have invalid asset"));

	return Asset->NodeCollection.Find(NodeId);
}

const FEventflowPinRelation* UEventflowEngine::GetPinRelation(const FGuid& PinId) const
{
	UEventflowAsset* Asset = GetAsset();
	checkf(IsValid(Asset), TEXT("Eventflow enigne have invalid asset"));

	return Asset->PinRelation.Find(PinId);
}


void UEventflowEngine::ReachNode(const FGuid& NodeId)
{
	const FEventflowNode* Node = GetNode(NodeId);
	checkf(Node, TEXT("Failed to find node with id"));
	
	_ActiveNodeId = NodeId;

	RemoveNodeTask();
	CreateNodeTask(NodeId, Node);
}

void UEventflowEngine::ReachEntryNode()
{
	UEventflowAsset* Asset = GetAsset();
	checkf(IsValid(Asset), TEXT("Eventflow enigne have invalid asset"));

	ReachNode(Asset->EntryNodeId);
}

void UEventflowEngine::ReachNextNode(int Index)
{
	const FEventflowNode* CurrentNode = GetNode(_ActiveNodeId);
	checkf(CurrentNode, TEXT("Current node is invalid"));

	const TArray<FEventflowPin>& Outputs = CurrentNode->StaticOutputs;
	checkf(!Outputs.IsEmpty(), TEXT("Current node have no output pins, if this was supposed to be exit point then change its node type to Exit"));
	checkf(Outputs.IsValidIndex(Index), TEXT("Current node has no output pins or invalid index"));

	const FEventflowPinRelation* Relation = GetPinRelation(Outputs[Index].UniqueId);
	checkf(Relation, TEXT("Failed to find pin relation for next node"));

	ReachNode(Relation->LinkedToNode);
}

void UEventflowEngine::ReachPreviousNode()
{
	const FEventflowNode* CurrentNode = GetNode(_ActiveNodeId);
	if (!CurrentNode)
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Failed to find node"));
		Finish(EFSMResult::Aborted);
		return;
	}

	const TArray<FEventflowPin>& Inputs = CurrentNode->StaticInputs;
	if (Inputs.Num() == 0)
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Failed to find input"));
		Finish(EFSMResult::Aborted);
		return;
	}

	const TMap<FGuid, FEventflowPinRelation>& PinRelation = _Asset->PinRelation;
	for (const TPair<FGuid, FEventflowPinRelation>& Kv : PinRelation)
	{
		if (Kv.Value.LinkedToPin == Inputs[0].UniqueId)
		{
			ReachNode(Kv.Value.LinkedToNode);
			return;
		}
	}

	LOG_ERROR(LogEventflowEngine, TEXT("Failed to find input relation"));
	Finish(EFSMResult::Aborted);
}


void UEventflowEngine::RemoveGlobalTasks()
{
	for (UEventflowGlobalTask* Task : _ActiveGlobalTasks)
	{
		Task->OnStateChanged.Unbind();

		if (Task->GetState() == EFSMState::Active)
		{
			Task->Finish(EFSMResult::Aborted);
		}
		if (Task->GetState() != EFSMState::Uninitialized)
		{
			Task->Reset();
		}
	}
	_ActiveGlobalTasks.Empty();
}


void UEventflowEngine::CreateNodeTask(const FGuid& NodeId, const FEventflowNode* Node)
{
	const UEventflowNodeTask* AssetTask = Node->Task;
	checkf(IsValid(AssetTask), TEXT("Node contains invalid instanced node task"));

	UClass* Class = AssetTask->GetClass();

	_ActiveNodeTask = FPoolLibrary::AcquireFromContainer<UEventflowNodeTask>(_NodeTaskPool, Class, this);
	_ActiveNodeTask->OnStateChanged.BindUObject(this, &UEventflowEngine::HandleOnNodeTaskStateChanged);
	_ActiveNodeTask->CopyFromAsset(AssetTask);
	_ActiveNodeTask->InitializeData(NodeId, Node);
	_ActiveNodeTask->Initialize();
}

void UEventflowEngine::RemoveNodeTask()
{
	if (IsValid(_ActiveNodeTask))
	{
		_ActiveNodeTask->OnStateChanged.Unbind();

		if (_ActiveNodeTask->GetState() == EFSMState::Active)
		{
			_ActiveNodeTask->Finish(EFSMResult::Aborted);
		}
		if (_ActiveNodeTask->GetState() != EFSMState::Uninitialized)
		{
			_ActiveNodeTask->Reset();
		}

		FPoolLibrary::ReturnToContainer(_NodeTaskPool, _ActiveNodeTask);
		LOG_WARNING(LogEventflowEngine, TEXT("Primary task removed and returned to pool"));
	}

	_ActiveNodeTask = nullptr;
}


void UEventflowEngine::CreateReturnData(UEventflowNodeTask* Task)
{
	Task->GetReturnData(_ReturnData);
}

void UEventflowEngine::RemoveReturnData()
{
	_ReturnData.Reset();
}


void UEventflowEngine::HandleOnNodeTaskStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result)
{
	if (NewState != EFSMState::Finished)
	{
		return;
	}

	UEventflowNodeTask* NodeTask = GetNodeTask();
	checkf(NodeTask, TEXT("Node task state changed but active node is invalid"));

	if (NodeTask->NodeType == EEventflowNodeType::Exit)
	{
		CreateReturnData(NodeTask);

		const FEventflowReturnData* ReturnData = GetReturnData().GetPtr();
		checkf(ReturnData, TEXT("Node must return a valid return data"));

		switch (ReturnData->GraphTransition)
		{
		case EEventflowGraphTransitionType::GraphSuccess:
			Finish(EFSMResult::Success);
			break;
		case EEventflowGraphTransitionType::GraphFail:
			Finish(EFSMResult::Failed);
			break;
		}
	}
	else
	{
		const FEventflowNodeTransitionData* TransitionData = NodeTask->GetTransitionData(Result).GetPtr();
		checkf(TransitionData, TEXT("Node must return a valid transition data"));

		switch (TransitionData->NodeTransition)
		{
		case EEventflowNodeTransitionType::NextNode:
			ReachNextNode(TransitionData->NextNodeIndex);
			break;
		case EEventflowNodeTransitionType::RedirectNode:
			ReachNode(TransitionData->NextNodeId);
			break;
		case EEventflowNodeTransitionType::RestartNode:
			NodeTask->Restart();
			break;
		}
	}
}


void UEventflowEngine::OnInitialized(EFSMState PreviousState)
{
	_AssetManager = UAssetManager::GetIfInitialized();

	checkf(_AssetId.IsValid(), TEXT("Invalid assetId provided to evenflow engine"));
	checkf(IsValid(_AssetManager), TEXT("Evenflow engine have invalid asset manager"));

	FAssetManagerLibrary::CancelHandle(_AssetHandle);

	TArray<FName> AssetBundle;
	GetAssetBundle(AssetBundle);

	_AssetHandle = _AssetManager->LoadPrimaryAsset(_AssetId, AssetBundle, FStreamableDelegate::CreateUObject(this, &UEventflowEngine::Load));
}

void UEventflowEngine::OnLoaded(EFSMState PreviousState)
{
	_Asset = _AssetManager->GetPrimaryAssetObject<UEventflowAsset>(_AssetId);
	checkf(IsValid(_Asset), TEXT("Failed to load asset"));

	Ready();
}

void UEventflowEngine::OnReady(EFSMState PreviousState)
{

}

void UEventflowEngine::OnActive(EFSMState PreviousState)
{
	switch (_EntryData.EntryType)
	{
	case EEventflowEntryType::Root:
		ReachEntryNode();
		break;

	case EEventflowEntryType::Custom:
		ReachNode(_EntryData.EntryNodeId);
		break;

	default:
		LOG_ERROR(LogEventflowEngine, TEXT("Unknown entry location"));
		break;
	}
}

void UEventflowEngine::OnEndActive(EFSMState NextState, EFSMResult Result)
{

}

void UEventflowEngine::OnFinished(EFSMResult Result)
{

}

void UEventflowEngine::OnRestart(EFSMState PreviousState, EFSMResult PreviousResult)
{

}

void UEventflowEngine::OnReset()
{
	RemoveReturnData();
	RemoveGlobalTasks();
	RemoveNodeTask();

	FPoolLibrary::Clear(_NodeTaskPool);

	_Asset = nullptr;
	FAssetManagerLibrary::CancelHandle(_AssetHandle);

	if (IsValid(_AssetManager))
	{
		_AssetManager->UnloadPrimaryAsset(_AssetId);
	}

	_AssetManager = nullptr;
	_AssetId = FPrimaryAssetId();

	_EntryData.Reset();
	_ReturnData.Reset();

	_ActiveNodeId.Invalidate();
}

