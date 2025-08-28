// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdGraphSchema.h"

// Engine Headers
#include "EdGraph/EdGraphNode.h"

// Project Headers
#include "RenEventflow/Public/EventflowNodeData.h"

#include "RenEventflowEditor/Public/Graph/EventflowEdGraph.h"
#include "RenEventflowEditor/Public/Graph/EventflowEdGraphNode.h"



const FName UEventflowEdGraphSchema::PC_Exec = FName("REN.EF.PC.EXEC");
const FName UEventflowEdGraphSchema::PC_Wildcard = FName("REN.EF.PC.WILDCARD");



void UEventflowEdGraphSchema::AddGraphNodeActions(FGraphContextMenuBuilder& ContextMenuBuilder, TSubclassOf<UEventflowEdGraphNode> NodeClass, const FString& InCategory, FText InMenuDesc, FText InToolTip) const
{
	TSharedPtr<FEventflowEdGraphSchemaAction> Action = MakeShareable(new FEventflowEdGraphSchemaAction(NodeClass, FText::FromString(InCategory), InMenuDesc, InToolTip, 0));
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

TArray<UClass*> UEventflowEdGraphSchema::GetNodeClasses() const
{
	return TArray<UClass*>();
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
	UEventflowEdGraph* Graph = Cast<UEventflowEdGraph>(ParentGraph);
	if (!Graph) return nullptr;

	UEventflowEdGraphNode* NewNode = NewObject<UEventflowEdGraphNode>(Graph, NodeClass);
	NewNode->CreateNewGuid();
	NewNode->AllocateDefaultPins();
	NewNode->NodePosX = Location.X;
	NewNode->NodePosY = Location.Y;

	TSubclassOf<UEventflowNodeData> NodeDataClass = NewNode->GetNodeDataClass();
	if (!NodeDataClass) return nullptr;

	NewNode->SetNodeData(NewObject<UEventflowNodeData>(NewNode, NodeDataClass));

	Graph->Modify();
	Graph->AddNode(NewNode, true, true);

	return NewNode;
}

