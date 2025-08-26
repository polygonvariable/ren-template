// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdGraphSchema.h"

// Engine Headers
#include "EdGraph/EdGraphNode.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEditor/Public/Graph/EventflowEdGraphNode.h"



void UEventflowEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	// AddGraphNodeActions(ContextMenuBuilder, UEventflowEdGraphNode::StaticClass(), TEXT("Nodes"), TEXT("Begin Node"), TEXT("Make a new begin node"));
	// AddGraphNodeActions(ContextMenuBuilder, UEventflowEdGraphNode::StaticClass(), TEXT("Nodes"), TEXT("End Node"), TEXT("Make a new end node"));
}

void UEventflowEdGraphSchema::AddGraphNodeActions(FGraphContextMenuBuilder& ContextMenuBuilder, TSubclassOf<UEventflowEdGraphNode> NodeClass, const FString& InCategory, const FString& InMenuDesc, const FString& InToolTip) const
{
	TSharedPtr<FEventflowEdGraphSchemaAction> Action = MakeShareable(new FEventflowEdGraphSchemaAction(
		NodeClass,
		FText::FromString(InCategory),
		FText::FromString(InMenuDesc),
		FText::FromString(InToolTip),
		0
	));
	ContextMenuBuilder.AddAction(Action);
}

const FPinConnectionResponse UEventflowEdGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (!A || !B)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect null pins"));
	}

	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect pins of same direction"));
	}

	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect pins of same node"));
	}
	
	return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
}



FEventflowEdGraphSchemaAction::FEventflowEdGraphSchemaAction()
{
}

FEventflowEdGraphSchemaAction::FEventflowEdGraphSchemaAction(TSubclassOf<UEventflowEdGraphNode> InNodeClass, FText InCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping) : FEdGraphSchemaAction(InCategory, InMenuDesc, InToolTip, InGrouping)
{
	NodeClass = InNodeClass;
}

UEdGraphNode* FEventflowEdGraphSchemaAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UEventflowEdGraphNode* NewNode = NewObject<UEventflowEdGraphNode>(ParentGraph, NodeClass);
	NewNode->CreateNewGuid();
	NewNode->NodePosX = Location.X;
	NewNode->NodePosY = Location.Y;
	NewNode->SetAssetNodeData(NewObject<UEventflowNodeData>(NewNode, GetAssetNodeDataClass()));

	ParentGraph->Modify();
	ParentGraph->AddNode(NewNode, true, true);

	return NewNode;
}

TSubclassOf<UEventflowNodeData> FEventflowEdGraphSchemaAction::GetAssetNodeDataClass() const
{
	return UEventflowNodeData::StaticClass();
}

