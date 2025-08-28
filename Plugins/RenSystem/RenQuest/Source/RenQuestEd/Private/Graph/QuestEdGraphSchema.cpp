// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/QuestEdGraphSchema.h"

// Engine Headers

// Project Headers
#include "RenQuestEd/Public/Graph/QuestEdGraphNode.h"



TArray<UClass*> UQuestEdGraphSchema::GetNodeClasses() const
{
	return {
		UQuestEdBeginNode::StaticClass(),
		UQuestEdObjectiveNode::StaticClass(),
		UQuestEdEndNode::StaticClass(),
	};
}

void UQuestEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
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

