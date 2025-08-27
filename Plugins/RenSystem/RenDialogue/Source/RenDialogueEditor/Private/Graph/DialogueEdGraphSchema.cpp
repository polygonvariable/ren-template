// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/DialogueEdGraphSchema.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenDialogue/Public/DialogueAsset.h"

#include "RenDialogueEditor/Public/Graph/DialogueEdGraphNode.h"



void UDialogueEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	AddGraphNodeActions(
		ContextMenuBuilder,
		UDialogueEdBeginNode::StaticClass(),
		TEXT("Nodes"),
		UDialogueEdBeginNode::StaticNodeTitle(),
		UDialogueEdBeginNode::StaticNodeDescription()
	);
	AddGraphNodeActions(
		ContextMenuBuilder,
		UDialogueEdDialogNode::StaticClass(),
		TEXT("Nodes"),
		UDialogueEdDialogNode::StaticNodeTitle(),
		UDialogueEdDialogNode::StaticNodeDescription()
	);
	AddGraphNodeActions(
		ContextMenuBuilder,
		UDialogueEdEndNode::StaticClass(),
		TEXT("Nodes"),
		UDialogueEdEndNode::StaticNodeTitle(),
		UDialogueEdEndNode::StaticNodeDescription()
	);
}

void UDialogueEdGraphSchema::AddGraphNodeActions(FGraphContextMenuBuilder& ContextMenuBuilder, TSubclassOf<UEventflowEdGraphNode> NodeClass, const FString& InCategory, FText InMenuDesc, FText InToolTip) const
{
	TSharedPtr<FDialogueEdGraphSchemaAction> Action = MakeShareable(new FDialogueEdGraphSchemaAction(NodeClass, FText::FromString(InCategory), InMenuDesc, InToolTip, 0));
	ContextMenuBuilder.AddAction(Action);
}



FDialogueEdGraphSchemaAction::FDialogueEdGraphSchemaAction()
{
}

FDialogueEdGraphSchemaAction::FDialogueEdGraphSchemaAction(TSubclassOf<UEventflowEdGraphNode> InNodeClass, FText InCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping) : FEventflowEdGraphSchemaAction(InNodeClass, InCategory, InMenuDesc, InToolTip, InGrouping)
{
}

TSubclassOf<UEventflowNodeData> FDialogueEdGraphSchemaAction::GetAssetNodeDataClass() const
{
	return UDialogueNodeData::StaticClass();
}

