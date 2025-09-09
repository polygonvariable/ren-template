// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEngine.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowBlueprint.h"
#include "RenEventflow/Public/EventflowData.h"
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowNodeData.h"
#include "RenEventflow/Public/EventflowNodeTask.h"



UEventflowAsset* UEventflowEngine::GetOwningAsset() const
{
	return CurrentAsset;
}

void UEventflowEngine::LoadAsset(TSoftObjectPtr<UEventflowAsset> InEventflowAsset)
{
	if (InEventflowAsset.IsNull())
	{
		LOG_ERROR(LogTemp, TEXT("InEventflowAsset is null"));
		return;
	}

	CurrentAssetPath = InEventflowAsset;

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(InEventflowAsset.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &UEventflowEngine::HandleAssetLoaded));
}

void UEventflowEngine::LoadAsset(UEventflowAsset* InEventflowAsset)
{
	if (!IsValid(InEventflowAsset))
	{
		LOG_ERROR(LogTemp, TEXT("InEventflowAsset is null"));
		return;
	}

	CurrentAsset = InEventflowAsset;

	InitializeEngine();
	HandleOnEngineInitialized();
}

void UEventflowEngine::UnloadAsset()
{
	DestructBlueprint();

	CurrentAsset = nullptr;

	if (CurrentAssetPath.IsNull())
	{
		LOG_ERROR(LogTemp, TEXT("CurrentAssetPath is null"));
		return;
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.Unload(CurrentAssetPath.ToSoftObjectPath());
}

void UEventflowEngine::InitializeEngine()
{
	if (!IsValid(CurrentAsset) || !CurrentAsset->GraphBlueprint)
	{
		LOG_ERROR(LogTemp, TEXT("Asset or GraphBlueprint is null"));
		return;
	}

	ConstructBlueprint(CurrentAsset->GraphBlueprint);
}

void UEventflowEngine::ReachEntryNode()
{
	if (!IsValid(CurrentAsset))
	{
		LOG_ERROR(LogTemp, TEXT("Asset is null"));
		return;
	};

	bool bAssetValid = CurrentAsset && CurrentAsset->GraphData;
	if (!bAssetValid)
	{
		LOG_ERROR(LogTemp, TEXT("GraphData is null"));
		return;
	}

	UEventflowData* GraphData = CurrentAsset->GraphData;
	ReachNode(GraphData->NodeEntry);
}

void UEventflowEngine::ReachNode(FGuid NodeID)
{
	if (!IsValid(CurrentAsset))
	{
		LOG_ERROR(LogTemp, TEXT("Asset is null"));
		return;
	};

	bool bAssetValid = CurrentAsset && CurrentAsset->GraphData;
	if (!bAssetValid)
	{
		LOG_ERROR(LogTemp, TEXT("GraphData is null"));
		return;
	}

	TMap<FGuid, TObjectPtr<UEventflowNode>>& NodeList = CurrentAsset->GraphData->NodeList;
	const TObjectPtr<UEventflowNode>* NodePtr = NodeList.Find(NodeID);

	if (!NodePtr)
	{
		LOG_ERROR(LogTemp, TEXT("Node is invalid"));
		return;
	}

	UEventflowNode* Node = NodePtr->Get();

	SetCurrentNode(Node);
}

void UEventflowEngine::ReachNextNode(int NextIndex)
{
	if (!IsValid(CurrentNode))
	{
		LOG_ERROR(LogTemp, TEXT("CurrentNode is null"));
		return;
	}

	UEventflowNode* Node = CurrentNode->GetNextNodeAt(NextIndex);
	SetCurrentNode(Node);
}

void UEventflowEngine::ReachImmediateNextNode()
{
	ReachNextNode(0);
}

void UEventflowEngine::ConstructBlueprint(TSubclassOf<UObject> InClass)
{
	UEventflowBlueprint* GraphBlueprint = NewObject<UEventflowBlueprint>(this, InClass);
	if (!IsValid(GraphBlueprint))
	{
		LOG_ERROR(LogTemp, TEXT("GraphBlueprint is null"));
		return;
	}

	CurrentBlueprint = GraphBlueprint;

	GraphBlueprint->OnNodeExecuteFinished.BindUObject(this, &UEventflowEngine::HandleOnNodeExited);
	GraphBlueprint->RegisterBlueprint(CurrentAsset);
}

void UEventflowEngine::DestructBlueprint()
{
	if (IsValid(CurrentBlueprint))
	{
		CurrentBlueprint->OnNodeExecuteFinished.Unbind();
		CurrentBlueprint->UnregisterBlueprint();
		CurrentBlueprint->MarkAsGarbage();
	}
	CurrentBlueprint = nullptr;
}

UEventflowNode* UEventflowEngine::GetCurrentNode()
{
	return CurrentNode;
}

UEventflowBlueprint* UEventflowEngine::GetCurrentBlueprint()
{
	return CurrentBlueprint;
}

void UEventflowEngine::SetCurrentNode(UEventflowNode* Node)
{
	if (!IsValid(Node) || (CurrentNode == Node))
	{
		LOG_ERROR(LogTemp, TEXT("Node is null or already current node"));
		HandleOnGraphEnded();
		return;
	}

	CurrentNode = Node;
	HandleOnNodeReached(Node);
}

bool UEventflowEngine::ExecuteNode(UEventflowNode* Node)
{
	if (!IsValid(CurrentBlueprint))
	{
		return false;
	}
	return CurrentBlueprint->StartNodeExecution(Node);
}

void UEventflowEngine::HandleAssetLoaded()
{
	CurrentAsset = CurrentAssetPath.Get();
	if (IsValid(CurrentAsset))
	{
		InitializeEngine();
		HandleOnEngineInitialized();
	}
}



void UEventflowEngine::HandleOnNodeReached(UEventflowNode* Node)
{
	OnNodeReached.ExecuteIfBound(Node);
}

void UEventflowEngine::HandleOnNodeExited(UEventflowNode* Node, bool bSuccess)
{
	OnNodeExited.ExecuteIfBound(Node, bSuccess);
}

void UEventflowEngine::HandleOnGraphEnded()
{
	OnGraphEnded.ExecuteIfBound();
}

void UEventflowEngine::HandleOnEngineInitialized()
{
	OnEngineInitialized.ExecuteIfBound();
}


