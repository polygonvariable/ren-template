// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEngine.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowBlueprint.h"
#include "RenEventflow/Public/EventflowData.h"
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowNodeData.h"
#include "RenEventflow/Public/EventflowNodeTask.h"



FPrimaryAssetId UEventflowEngine::GetOwningAssetId() const
{
	return CurrentAssetId;
}

UEventflowAsset* UEventflowEngine::GetOwningAsset() const
{
	return CurrentAsset;
}

void UEventflowEngine::LoadAsset(const FPrimaryAssetId& AssetId)
{
	if (IsValid(CurrentAsset))
	{
		LOG_ERROR(LogTemp, TEXT("Asset is already loaded"));
		return;
	}

	if (!AssetId.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("AssetId is invalid"));
		return;
	}

	CurrentAssetId = AssetId;

	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (IsValid(AssetManager))
	{
		AssetManager->LoadPrimaryAsset(AssetId, TArray<FName>{}, FStreamableDelegate::CreateUObject(this, &UEventflowEngine::HandleAssetLoaded));
	}
}

void UEventflowEngine::LoadAsset(UEventflowAsset* Asset)
{
	if (IsValid(CurrentAsset))
	{
		LOG_ERROR(LogTemp, TEXT("Asset is already loaded"));
		return;
	}

	if (!IsValid(Asset))
	{
		LOG_ERROR(LogTemp, TEXT("Asset is invalid"));
		return;
	}

	CurrentAsset = Asset;

	InitializeEngine();
	HandleOnGraphStarted();
}

void UEventflowEngine::UnloadAsset()
{
	DestructBlueprint();

	CachedGraphData = nullptr;
	CurrentAsset = nullptr;

	if (CurrentAssetId.IsValid())
	{
		UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
		if (IsValid(AssetManager))
		{
			AssetManager->UnloadPrimaryAsset(CurrentAssetId);
		}
	}
}

UEventflowNode* UEventflowEngine::GetNodeById(FGuid NodeId) const
{
	if (!IsValid(CachedGraphData))
	{
		LOG_ERROR(LogTemp, TEXT("CachedGraphData is invalid"));
		return nullptr;
	};

	TMap<FGuid, TObjectPtr<UEventflowNode>>& NodeList = CachedGraphData->NodeList;
	const TObjectPtr<UEventflowNode>* NodeObject = NodeList.Find(NodeId);

	if (!NodeObject)
	{
		LOG_ERROR(LogTemp, TEXT("Node is invalid"));
		return nullptr;
	}

	return NodeObject->Get();
}

void UEventflowEngine::InitializeEngine()
{
	if (!IsValid(CurrentAsset))
	{
		LOG_ERROR(LogTemp, TEXT("Asset invalid"));
		return;
	}

	CachedGraphData = CurrentAsset->GraphData;

	if (!CurrentAsset->GraphBlueprint)
	{
		LOG_ERROR(LogTemp, TEXT("GraphBlueprint is invalid"));
		return;
	}

	ConstructBlueprint(CurrentAsset->GraphBlueprint);
}

bool UEventflowEngine::ReachEntryNode()
{
	if (!IsValid(CachedGraphData))
	{
		LOG_ERROR(LogTemp, TEXT("CachedGraphData is invalid"));
		return false;
	};

	TMap<FGuid, TObjectPtr<UEventflowNode>>& NodeList = CachedGraphData->NodeList;
	const TObjectPtr<UEventflowNode>* NodeObject = NodeList.Find(CachedGraphData->NodeEntry);

	if (!NodeObject)
	{
		LOG_ERROR(LogTemp, TEXT("Node is invalid"));
		return false;
	}

	UEventflowNode* Node = NodeObject->Get();
	return ReachNode(Node);
}

bool UEventflowEngine::ReachNode(UEventflowNode* Node)
{
	if (!IsValid(Node))
	{
		LOG_ERROR(LogTemp, TEXT("Node is invalid"));
		return false;
	}

	HandleOnNodeReached(Node);
	return true;
}

bool UEventflowEngine::ReachNodeById(FGuid NodeId)
{
	UEventflowNode* Node = GetNodeById(NodeId);
	return ReachNode(Node);
}

bool UEventflowEngine::ReachNextNode(UEventflowNode* Node, int Index)
{
	if (!IsValid(Node))
	{
		LOG_ERROR(LogTemp, TEXT("Node is invalid"));
		return false;
	}

	UEventflowNode* ReachedNode = Node->GetNextNodeAt(Index);
	if (!IsValid(ReachedNode))
	{
		LOG_ERROR(LogTemp, TEXT("ReachedNode is invalid"));
		return false;
	}

	return ReachNode(ReachedNode);
}

bool UEventflowEngine::ReachImmediateNextNode(UEventflowNode* Node)
{
	return ReachNextNode(Node, 0);
}

void UEventflowEngine::ConstructBlueprint(TSubclassOf<UObject> InClass)
{
	if (!InClass)
	{
		LOG_ERROR(LogTemp, TEXT("InClass is invalid"));
		return;
	}

	UEventflowBlueprint* GraphBlueprint = NewObject<UEventflowBlueprint>(this, InClass);
	if (!IsValid(GraphBlueprint))
	{
		LOG_ERROR(LogTemp, TEXT("GraphBlueprint is invalid"));
		return;
	}

	CurrentBlueprint = GraphBlueprint;

	GraphBlueprint->OnNodeExited.BindUObject(this, &UEventflowEngine::HandleOnNodeExited);
	GraphBlueprint->RegisterBlueprint(CurrentAsset);
}

void UEventflowEngine::DestructBlueprint()
{
	if (IsValid(CurrentBlueprint))
	{
		CurrentBlueprint->OnNodeExited.Unbind();
		CurrentBlueprint->UnregisterBlueprint();
		CurrentBlueprint->MarkAsGarbage();
	}
	CurrentBlueprint = nullptr;
}

UEventflowBlueprint* UEventflowEngine::GetCurrentBlueprint() const
{
	return CurrentBlueprint;
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
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!IsValid(AssetManager))
	{
		LOG_ERROR(LogTemp, TEXT("AssetManager is invalid"));
		return;
	}

	UObject* LoadedAsset = AssetManager->GetPrimaryAssetObject(CurrentAssetId);
	if (!IsValid(LoadedAsset))
	{
		LOG_ERROR(LogTemp, TEXT("LoadedAsset is invalid"));
		return;
	}

	CurrentAsset = Cast<UEventflowAsset>(LoadedAsset);
	if (!IsValid(CurrentAsset))
	{
		LOG_ERROR(LogTemp, TEXT("CurrentAsset is invalid"));
		return;
	}

	InitializeEngine();
	HandleOnGraphStarted();
}



void UEventflowEngine::HandleOnNodeReached(UEventflowNode* Node)
{
}

void UEventflowEngine::HandleOnNodeExited(UEventflowNode* Node, bool bSuccess, int NextNodeIndex)
{
}

void UEventflowEngine::HandleOnGraphStarted()
{
	OnGraphStarted.ExecuteIfBound();
}

void UEventflowEngine::HandleOnGraphEnded()
{
	OnGraphEnded.ExecuteIfBound();
}


