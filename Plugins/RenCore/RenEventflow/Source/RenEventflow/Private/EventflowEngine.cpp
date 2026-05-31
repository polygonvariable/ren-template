// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEngine.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "EventflowAsset.h"
#include "EventflowTask.h"
#include "Library/AssetManagerUtil.h"
#include "Library/PoolHelper.h"
#include "Log/LogMacro.h"


void UEventflowEngine::InitializeEngine(const FPrimaryAssetId& AssetId)
{
	AssetManager = UAssetManager::GetIfInitialized();

	if (CurrentAssetId.IsValid() || IsValid(CurrentAsset))
	{
		Fail(TEXT("Engine is already initialized"));
		return;
	}

	if (!AssetId.IsValid() || !IsValid(AssetManager))
	{
		Fail(TEXT("AssetId, AssetManager is invalid"));
		return;
	}

	CurrentAssetId = AssetId;

	FAssetManagerUtil::CancelHandle(_SpawnHandle);

	_SpawnHandle = AssetManager->LoadPrimaryAsset(CurrentAssetId, TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UEventflowEngine::HandleInitialization));
}

void UEventflowEngine::DeinitializeEngine()
{
	CurrentAsset = nullptr;
	CurrentNodeId = FGuid();

	FAssetManagerUtil::CancelHandle(_SpawnHandle);
	AssetManager->UnloadPrimaryAsset(CurrentAssetId);
	AssetManager = nullptr;

	CurrentAssetId = FPrimaryAssetId();
}

void UEventflowEngine::HandleInitialization()
{
	FAssetManagerUtil::ReleaseHandle(_SpawnHandle);

	CurrentAsset = AssetManager->GetPrimaryAssetObject<UEventflowAsset>(CurrentAssetId);
	if (!IsValid(CurrentAsset))
	{
		Fail(TEXT("Failed to load asset"));
		return;
	}
	StartEngine();
}


void UEventflowEngine::StartEngine()
{
	OnStarted.ExecuteIfBound();
	ReachEntryNode();
}

void UEventflowEngine::StopEngine(bool bInterrupted)
{
	RemoveActiveTask();
	OnEnded.ExecuteIfBound();
}


const FEventflowNodeDefinition* UEventflowEngine::GetNode(const FGuid& NodeId) const
{
	return CurrentAsset->NodeCollection.Find(NodeId);
}

const FEventflowPinRelation* UEventflowEngine::GetPinRelation(const FGuid& PinId) const
{
	return CurrentAsset->PinRelation.Find(PinId);
}


void UEventflowEngine::ReachNode(const FGuid& NodeId)
{
	const FEventflowNodeDefinition* Node = GetNode(NodeId);
	if (!Node)
	{
		Fail(TEXT("Failed to find entry node"));
		return;
	}

	CurrentNodeId = NodeId;

	RemoveActiveTask();
	CreateActiveTask(NodeId, Node);
}

void UEventflowEngine::ReachEntryNode()
{
	ReachNode(CurrentAsset->EntryNodeId);
}

void UEventflowEngine::ReachNextNode(int Index)
{
	const FEventflowNodeDefinition* CurrentNode = GetNode(CurrentNodeId);
	if (!CurrentNode)
	{
		Fail(TEXT("Failed to find node"));
		return;
	}

	const TArray<FEventflowPinDefinition>& Outputs = CurrentNode->StaticOutputs;
	if (Outputs.Num() == 0)
	{
		LOG_WARNING(LogTemp, TEXT("Node has no outputs, stopping engine"));

		StopEngine(false);
		return;
	}

	if (!Outputs.IsValidIndex(Index))
	{
		Fail(TEXT("Invalid output index"));
		return;
	}

	const FEventflowPinRelation* Relation = GetPinRelation(Outputs[Index].UniqueId);
	if (!Relation)
	{
		Fail(TEXT("Failed to find output relation"));
		return;
	}

	ReachNode(Relation->LinkedToNode);
}


void UEventflowEngine::CreateActiveTask(const FGuid& NodeId, const FEventflowNodeDefinition* Node)
{
	if (!IsValid(Node->PrimaryTask))
	{
		LOG_WARNING(LogTemp, TEXT("Task class is invalid, no task will be created"));
		return;
	}

	UClass* TaskClass = Node->PrimaryTask->GetClass();

	CurrentTask = FPoolHelper::AcquireFromContainer<UEventflowTask>(_TaskPool, TaskClass, this);
	CurrentTask->CopyFromTemplate(Node->PrimaryTask);
	CurrentTask->OnTaskFinished.BindUObject(this, &UEventflowEngine::HandleTaskFinished);
	CurrentTask->InitializeTask(NodeId, Node);
}

void UEventflowEngine::RemoveActiveTask()
{
	if (IsValid(CurrentTask))
	{
		CurrentTask->OnTaskFinished.Unbind();
		CurrentTask->DeinitializeTask();
		FPoolHelper::ReturnToContainer(_TaskPool, CurrentTask);
	}
	CurrentTask = nullptr;
}

void UEventflowEngine::HandleTaskFinished(EEventflowDirection Direction, int Index)
{
	if (Direction == EEventflowDirection::Next)
	{
		RemoveActiveTask();
		ReachNextNode(Index);
	}
}


void UEventflowEngine::Fail(const FString& Message)
{
	LOG_ERROR(LogTemp, TEXT("%s"), *Message);
	StopEngine(true);
}


