// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEngine.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowBlueprint.h"
#include "RenEventflow/Public/EventflowData.h"
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowNodeData.h"
#include "RenEventflow/Public/EventflowNodeTask.h"



void UEventflowEngine::LoadAsset(TSoftObjectPtr<UEventflowAsset> InEventflowAsset)
{
	if (InEventflowAsset.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("UEventflowEngine::LoadAsset: InEventflowAsset is null"));
		return;
	}

	CurrentAsset = InEventflowAsset;

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(InEventflowAsset.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &UEventflowEngine::HandleAssetLoaded));
}

void UEventflowEngine::UnloadAsset()
{
	if (IsValid(CurrentBlueprint))
	{
		CurrentBlueprint->UnregisterGraph();
		CurrentBlueprint->MarkAsGarbage();
	}
	CurrentBlueprint = nullptr;
	CurrentNode = nullptr;

	if (CurrentAsset.IsNull())
	{
		UE_LOG(LogTemp, Error, TEXT("UEventflowEngine::UnloadAsset: CurrentAsset is null"));
		return;
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.Unload(CurrentAsset.ToSoftObjectPath());
}

void UEventflowEngine::InitializeEngine()
{
	UEventflowAsset* Asset = CurrentAsset.Get();
	if (!IsValid(Asset) || !Asset->GraphBlueprint)
	{
		UE_LOG(LogTemp, Error, TEXT("UEventflowEngine::InitializeEngine: Asset or GraphBlueprint is null"));
		return;
	}

	UEventflowBlueprint* GraphBlueprint = NewObject<UEventflowBlueprint>(this, Asset->GraphBlueprint);
	if (!IsValid(GraphBlueprint))
	{
		UE_LOG(LogTemp, Error, TEXT("UEventflowEngine::InitializeEngine: GraphBlueprint is null"));
		return;
	}

	CurrentBlueprint = GraphBlueprint;
	CurrentBlueprint->RegisterGraph(Asset);
	CurrentBlueprint->OnNodeExecuteFinished.BindWeakLambda(this, [this](UEventflowNode* Node, bool bSuccess)
		{
			OnNodeExited.ExecuteIfBound(Node, bSuccess);
		}
	);
}

void UEventflowEngine::ReachEntryNode()
{
	UEventflowAsset* Asset = CurrentAsset.Get();
	if (!IsValid(Asset))
	{
		UE_LOG(LogTemp, Error, TEXT("UEventflowEngine::ReachEntryNode: Asset is null"));
		return;
	};

	bool bAssetValid = Asset && Asset->GraphData;
	if (!bAssetValid)
	{
		UE_LOG(LogTemp, Error, TEXT("UEventflowEngine::ReachEntryNode: GraphData is null"));
		return;
	}

	UEventflowData* GraphData = Asset->GraphData;
	ReachNodeAt(GraphData->EntryNodeIndex);
}

void UEventflowEngine::ReachNodeAt(int Index)
{
	UEventflowAsset* Asset = CurrentAsset.Get();
	if (!IsValid(Asset))
	{
		UE_LOG(LogTemp, Error, TEXT("UEventflowEngine::ReachEntryNode: Asset is null"));
		return;
	};

	bool bAssetValid = Asset && Asset->GraphData;
	if (!bAssetValid)
	{
		UE_LOG(LogTemp, Error, TEXT("UEventflowEngine::ReachEntryNode: GraphData is null"));
		return;
	}

	UEventflowData* GraphData = Asset->GraphData;

	bool bDataValid = GraphData->Nodes.IsValidIndex(Index);
	if (!bDataValid)
	{
		UE_LOG(LogTemp, Error, TEXT("UEventflowEngine::ReachEntryNode: Index is invalid"));
		return;
	}

	UEventflowNode* Node = GraphData->Nodes[Index];
	SetCurrentNode(Node);
}

void UEventflowEngine::ReachNextNode(int Index)
{
	if (!IsValid(CurrentNode))
	{
		UE_LOG(LogTemp, Error, TEXT("UEventflowEngine::ReachNextNode: CurrentNode is null"));
		return;
	}

	UEventflowNode* Node = CurrentNode->GetNextNodeAt(Index);
	SetCurrentNode(Node);
}

void UEventflowEngine::ReachImmediateNextNode()
{
	ReachNextNode(0);
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
		UE_LOG(LogTemp, Error, TEXT("UEventflowEngine::SetCurrentNode: Node is invalid"));
		OnGraphEnded.ExecuteIfBound();
		return;
	}

	CurrentNode = Node;
	OnNodeReached.ExecuteIfBound(Node);
}

bool UEventflowEngine::StartNodeExecution(UEventflowNode* Node)
{
	return CurrentBlueprint->StartNodeExecution(Node);
}

void UEventflowEngine::HandleAssetLoaded()
{
	UEventflowAsset* Asset = CurrentAsset.Get();
	if (IsValid(Asset))
	{
		InitializeEngine();

		OnEngineInitialized.ExecuteIfBound();
	}
}

