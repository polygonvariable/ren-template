// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/StoryboardEdGraphSchema.h"

// Engine Headers

// Project Headers
#include "RenQuestEd/Public/Graph/StoryboardEdGraphNode.h"



TArray<UClass*> UStoryboardEdGraphSchema::GetNodeClasses() const
{
	return {
		UStoryboardEdBeginNode::StaticClass(),
		UStoryboardEdQuestNode::StaticClass(),
	};
}

void UStoryboardEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
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

