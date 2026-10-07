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


void UEventflowEngine::InitializeData(const FPrimaryAssetId& InAssetId, const FEventflowEntryData& InEntryData)
{
	_AssetId = InAssetId;
	_EntryData = InEntryData;
}


UWorld* UEventflowEngine::GetWorld() const
{
	return GetOuter()->GetWorld();
}

UEventflowNodeTask* UEventflowEngine::GetTask() const
{
	return _ActiveTask.Get();
}

UEventflowAsset* UEventflowEngine::GetAsset() const
{
	return _Asset;
}

const TInstancedStruct<FEventflowReturnData>& UEventflowEngine::GetReturnData() const
{
	return _ReturnData;
}


void UEventflowEngine::GetAssetBundle(TArray<FName>& OutBundle) const
{
}

const FEventflowNode* UEventflowEngine::GetNode(const FGuid& NodeId) const
{
	checkf(IsValid(_Asset), TEXT("Invalid asset"));
	return _Asset->NodeCollection.Find(NodeId);
}

const FEventflowPinRelation* UEventflowEngine::GetPinRelation(const FGuid& PinId) const
{
	checkf(IsValid(_Asset), TEXT("Invalid asset"));
	return _Asset->PinRelation.Find(PinId);
}


void UEventflowEngine::ReachNode(const FGuid& NodeId)
{
	const FEventflowNode* Node = GetNode(NodeId);
	checkf(Node, TEXT("Failed to found node with id"));
	
	//if (!Node)
	//{
	//	LOG_ERROR(LogEventflowEngine, TEXT("Failed to find entry node"));
	//	Finish(EFSMResult::Aborted);
	//	return;
	//}

	_ActiveNodeId = NodeId;

	RemoveTask();
	CreateTask(NodeId, Node);
}

void UEventflowEngine::ReachEntryNode()
{
	checkf(IsValid(_Asset), TEXT("Invalid asset"));
	ReachNode(_Asset->EntryNodeId);
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

	//if (!CurrentNode)
	//{
	//	LOG_ERROR(LogEventflowEngine, TEXT("Failed to find node"));
	//	Finish(EFSMResult::Aborted);
	//	return;
	//}

	//if (Outputs.Num() == 0)
	//{
	//	LOG_WARNING(LogEventflowEngine, TEXT("Failed to find next linked node, stopping graph with success"));
	//	Finish(EFSMResult::Success);
	//	return;
	//}

	//if (!Outputs.IsValidIndex(Index))
	//{
	//	LOG_ERROR(LogEventflowEngine, TEXT("Invalid output index"));
	//	Finish(EFSMResult::Aborted);
	//	return;
	//}

	//if (!Relation)
	//{
	//	LOG_ERROR(LogEventflowEngine, TEXT("Failed to find output relation"));
	//	Finish(EFSMResult::Aborted);
	//	return;
	//}

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


void UEventflowEngine::CreateTask(const FGuid& NodeId, const FEventflowNode* Node)
{
	const UEventflowNodeTask* AssetTask = Node->Task;
	checkf(IsValid(AssetTask), TEXT("Node contains invalid instanced node task"));

	UClass* Class = AssetTask->GetClass();

	_ActiveTask = FPoolLibrary::AcquireFromContainer<UEventflowNodeTask>(_TaskPool, Class, this);
	_ActiveTask->OnStateChanged.BindUObject(this, &UEventflowEngine::HandleOnTaskStateChanged);
	_ActiveTask->CopyFromAsset(AssetTask);
	_ActiveTask->InitializeData(NodeId, Node);
	_ActiveTask->Initialize();
}

void UEventflowEngine::RemoveTask()
{
	if (IsValid(_ActiveTask))
	{
		_ActiveTask->OnStateChanged.Unbind();

		if (_ActiveTask->GetState() == EFSMState::Active)
		{
			_ActiveTask->Finish(EFSMResult::Aborted);
		}
		if (_ActiveTask->GetState() != EFSMState::Uninitialized)
		{
			_ActiveTask->Reset();
		}

		FPoolLibrary::ReturnToContainer(_TaskPool, _ActiveTask);
		LOG_WARNING(LogEventflowEngine, TEXT("Primary task removed and returned to pool"));
	}

	_ActiveTask = nullptr;
}


void UEventflowEngine::CreateReturnData(UEventflowNodeTask* Task)
{
	Task->GetReturnData(_ReturnData);
}

void UEventflowEngine::RemoveReturnData()
{
	_ReturnData.Reset();
}


void UEventflowEngine::HandleOnTaskStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result)
{
	FString TaskState = UEnum::GetDisplayValueAsText(NewState).ToString();

	if (NewState == EFSMState::Finished)
	{
		UEventflowNodeTask* Task = GetTask();
		checkf(Task, TEXT("No active task found when its state changed"));

		if (Task->NodeType == EEventflowNodeType::Exit)
		{
			CreateReturnData(Task);

			const FEventflowReturnData* ReturnData = GetReturnData().GetPtr();
			checkf(ReturnData, TEXT("Node return data is invalid"));

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
			const FEventflowNodeTransitionData* TransitionData = Task->GetTransitionData(Result).GetPtr();
			checkf(TransitionData, TEXT("Node transition data is invalid"));

			switch (TransitionData->NodeTransition)
			{
			case EEventflowNodeTransitionType::NextNode:
				ReachNextNode(TransitionData->NextNodeIndex);
				break;
			case EEventflowNodeTransitionType::RedirectNode:
				ReachNode(TransitionData->NextNodeId);
				break;
			case EEventflowNodeTransitionType::RestartNode:
				Task->Restart();
				break;
			}
		}
	}
}


void UEventflowEngine::OnInitialized(EFSMState PreviousState)
{
	_AssetManager = UAssetManager::GetIfInitialized();
	checkf(_AssetId.IsValid(), TEXT("AssetId is invalid"));
	checkf(IsValid(_AssetManager), TEXT("Asset manager invalid"));

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
	Execute();
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
	RemoveTask();

	FPoolLibrary::Clear(_TaskPool);

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

