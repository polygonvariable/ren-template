// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEngine.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "EventflowAsset.h"
#include "Core/AssetManagerUtil.h"
#include "Library/PoolHelper.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Task/EventflowPrimaryTask.h"


void UEventflowEngine::InitializeData(const FPrimaryAssetId& AssetId, const FEventflowEntry& EntryDefinition)
{
	_AssetId = AssetId;
	_Entry = EntryDefinition;
}


UWorld* UEventflowEngine::GetWorld() const
{
	return GetOuter()->GetWorld();
}

UEventflowPrimaryTask* UEventflowEngine::GetTask() const
{
	return _ActiveTask.Get();
}

UEventflowAsset* UEventflowEngine::GetAsset() const
{
	return _Asset;
}


void UEventflowEngine::GetAssetBundle(TArray<FName>& OutBundle) const
{
}


const FEventflowNode* UEventflowEngine::GetNode(const FGuid& NodeId) const
{
	return _Asset->NodeCollection.Find(NodeId);
}

const FEventflowPinRelation* UEventflowEngine::GetPinRelation(const FGuid& PinId) const
{
	return _Asset->PinRelation.Find(PinId);
}


void UEventflowEngine::ReachNode(const FGuid& NodeId)
{
	const FEventflowNode* Node = GetNode(NodeId);
	if (!Node)
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Failed to find entry node"));
		Finish(EFSMResult::Failed);
		return;
	}

	_ActiveNodeId = NodeId;

	RemoveTask();
	CreateTask(NodeId, Node);
}

void UEventflowEngine::ReachEntryNode()
{
	ReachNode(_Asset->EntryNodeId);
}

void UEventflowEngine::ReachNextNode(int Index)
{
	const FEventflowNode* CurrentNode = GetNode(_ActiveNodeId);
	if (!CurrentNode)
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Failed to find node"));
		Finish(EFSMResult::Failed);
		return;
	}

	const TArray<FEventflowPin>& Outputs = CurrentNode->StaticOutputs;
	if (Outputs.Num() == 0)
	{
		LOG_WARNING(LogEventflowEngine, TEXT("Failed to find next linked node, stopping graph with success"));
		Finish(EFSMResult::Success);
		return;
	}

	if (!Outputs.IsValidIndex(Index))
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Invalid output index"));
		Finish(EFSMResult::Failed);
		return;
	}

	const FEventflowPinRelation* Relation = GetPinRelation(Outputs[Index].UniqueId);
	if (!Relation)
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Failed to find output relation"));
		Finish(EFSMResult::Failed);
		return;
	}

	ReachNode(Relation->LinkedToNode);
}

void UEventflowEngine::ReachPreviousNode()
{
	const FEventflowNode* CurrentNode = GetNode(_ActiveNodeId);
	if (!CurrentNode)
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Failed to find node"));
		Finish(EFSMResult::Failed);
		return;
	}

	const TArray<FEventflowPin>& Inputs = CurrentNode->StaticInputs;
	if (Inputs.Num() == 0)
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Failed to find input"));
		Finish(EFSMResult::Failed);
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
	Finish(EFSMResult::Failed);
}


void UEventflowEngine::CreateTask(const FGuid& NodeId, const FEventflowNode* Node)
{
	const UEventflowPrimaryTask* AssetTask = Node->Task;
	if (!IsValid(AssetTask))
	{
		LOG_WARNING(LogEventflowEngine, TEXT("Primary task is invalid"));
		return;
	}

	UClass* Class = AssetTask->GetClass();

	_ActiveTask = FPoolHelper::AcquireFromContainer<UEventflowPrimaryTask>(_TaskPool, Class, this);
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

		FPoolHelper::ReturnToContainer(_TaskPool, _ActiveTask);
		LOG_WARNING(LogEventflowEngine, TEXT("Primary task removed and returned to pool"));
	}

	_ActiveTask = nullptr;
}

void UEventflowEngine::HandleOnTaskStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result)
{
	FString TaskState = UEnum::GetDisplayValueAsText(NewState).ToString();

	if (NewState == EFSMState::Finished)
	{
		UEventflowPrimaryTask* Task = GetTask();
		if (IsValid(Task))
		{
			int TransitionIndex = Task->GetTransitionIndex(Result);
			EEventflowTransitionType TransitionType = Task->GetTransitionType(Result);

			switch (TransitionType)
			{
			case EEventflowTransitionType::GraphFail:
				LOG_ERROR(LogEventflowEngine, TEXT("Graph failed caused by task transition"));
				Finish(EFSMResult::Failed);
				break;
			case EEventflowTransitionType::GraphSuccess:
				LOG_WARNING(LogEventflowEngine, TEXT("Graph success caused by task transition"))
				Finish(EFSMResult::Success);
				break;
			case EEventflowTransitionType::NextNode:
				ReachNextNode(TransitionIndex);
				break;
			case EEventflowTransitionType::RestartNode:
				Task->Restart();
				break;
			default:
				LOG_ERROR(LogTemp, TEXT("Unknown transition type"));
				break;
			}
		}
		else
		{
			LOG_ERROR(LogEventflowEngine, TEXT("Primary task is invalid, no task will be handled"));
			Finish(EFSMResult::Failed);
		}
	}
}


void UEventflowEngine::OnInitialized(EFSMState PreviousState)
{
	_AssetManager = UAssetManager::GetIfInitialized();
	if (!_AssetId.IsValid() || !IsValid(_AssetManager))
	{
		LOG_ERROR(LogEventflowEngine, TEXT("AssetId or _AssetManager is invalid"));
		Finish(EFSMResult::Aborted);
		return;
	}

	FAssetManagerUtil::CancelHandle(_AssetHandle);

	TArray<FName> AssetBundle;
	GetAssetBundle(AssetBundle);

	_AssetHandle = _AssetManager->LoadPrimaryAsset(_AssetId, AssetBundle, FStreamableDelegate::CreateUObject(this, &UEventflowEngine::Load));
}

void UEventflowEngine::OnLoaded(EFSMState PreviousState)
{
	FAssetManagerUtil::ReleaseHandle(_AssetHandle);

	_Asset = _AssetManager->GetPrimaryAssetObject<UEventflowAsset>(_AssetId);
	if (!IsValid(_Asset))
	{
		LOG_ERROR(LogEventflowEngine, TEXT("Failed to load asset"));
		Finish(EFSMResult::Aborted);
		return;
	}

	Ready();
}

void UEventflowEngine::OnReady(EFSMState PreviousState)
{
	Execute();
}

void UEventflowEngine::OnActive(EFSMState PreviousState)
{
	switch (_Entry.EntryType)
	{
	case EEventflowEntryType::Root:
		ReachEntryNode();
		break;
	case EEventflowEntryType::Custom:
		ReachNode(_Entry.NodeId);
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
	RemoveTask();

	FAssetManagerUtil::CancelHandle(_AssetHandle);
	FPoolHelper::Clear(_TaskPool);

	_Asset = nullptr;

	if (IsValid(_AssetManager))
	{
		_AssetManager->UnloadPrimaryAsset(_AssetId);
	}
	_AssetManager = nullptr;

	_AssetId = FPrimaryAssetId();
	_Entry.Reset();

	_ActiveNodeId.Invalidate();
}

