// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/DialogueEdGraphSchema.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenDialogue/Public/DialogueAsset.h"

#include "RenDialogueEditor/Public/Graph/DialogueEdGraphNode.h"



TArray<UClass*> UDialogueEdGraphSchema::GetNodeClasses() const
{
	return {
		UDialogueEdBeginNode::StaticClass(),
		UDialogueEdDialogNode::StaticClass(),
		UDialogueEdEndNode::StaticClass()
	};
}

void UDialogueEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	const TArray<UClass*> NodeClasses = GetNodeClasses();

	for (UClass* NodeClass : NodeClasses)
	{
		UEventflowEdGraphNode* Node = NodeClass->GetDefaultObject<UEventflowEdGraphNode>();
		if (!Node) continue;

		AddGraphNodeActions(
			ContextMenuBuilder,
		    NodeClass,
			TEXT("Nodes"),
			Node->GetNodeTitle(ENodeTitleType::FullTitle),
			Node->GetNodeDescription()
		);
	}
}

