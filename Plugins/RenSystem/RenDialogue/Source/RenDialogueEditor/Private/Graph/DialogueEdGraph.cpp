// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/DialogueEdGraph.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenDialogue/Public/DialogueAsset.h"

#include "RenDialogueEditor/Public/Graph/DialogueEdGraphNode.h"



void UDialogueEdGraph::RegisterNodeTypes()
{
	RegisterNodeType(UDialogueEdBeginNode::StaticNodeType(), UDialogueEdBeginNode::StaticClass());
	RegisterNodeType(UDialogueEdDialogNode::StaticNodeType(), UDialogueEdDialogNode::StaticClass());
	RegisterNodeType(UDialogueEdEndNode::StaticNodeType(), UDialogueEdEndNode::StaticClass());
}

TSubclassOf<UEventflowNodeData> UDialogueEdGraph::GetAssetNodeDataClass() const
{
	return UDialogueNodeData::StaticClass();
}

