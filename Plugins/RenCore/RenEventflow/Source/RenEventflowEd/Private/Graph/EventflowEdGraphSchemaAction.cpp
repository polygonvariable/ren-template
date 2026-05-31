// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdGraphSchemaAction.h"

// Engine Headers
#include "EdGraph/EdGraphNode.h"

// Project Headers
#include "Graph/EventflowEdGraph.h"
#include "Graph/EventflowEdGraphNode.h"


FEventflowEdGraphSchemaAction::FEventflowEdGraphSchemaAction()
{
}

FEventflowEdGraphSchemaAction::FEventflowEdGraphSchemaAction(TSubclassOf<UEventflowEdGraphNode> InNodeClass, FText InCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping) : FEdGraphSchemaAction(InCategory, InMenuDesc, InToolTip, InGrouping)
{
	NodeClass = InNodeClass;
}

UEdGraphNode* FEventflowEdGraphSchemaAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UEventflowEdGraph* Graph = Cast<UEventflowEdGraph>(ParentGraph);
	if (!IsValid(Graph))
	{
		return nullptr;
	}

	UEventflowEdGraphNode* NewNode = NewObject<UEventflowEdGraphNode>(Graph, NodeClass);
	NewNode->CreateNewGuid();
	NewNode->AllocateDefaultPins();
	NewNode->NodePosX = Location.X;
	NewNode->NodePosY = Location.Y;

	Graph->Modify();
	Graph->AddNode(NewNode, true, true);

	return NewNode;
}

