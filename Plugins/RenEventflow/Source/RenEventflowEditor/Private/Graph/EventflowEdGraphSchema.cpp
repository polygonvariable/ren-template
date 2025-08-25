// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdGraphSchema.h"

// Engine Headers
#include "EdGraph/EdGraphNode.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEditor/Public/App/EventflowEdApp.h"
#include "RenEventflowEditor/Public/Graph/EventflowEdGraphNode.h"



void UEventflowEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FEventflowEdGraphSchemaAction> BeginNodeAction = MakeShareable(new FEventflowEdGraphSchemaAction(
		UEventflowEdGraphBeginNode::StaticClass(),
		FText::FromString(TEXT("Nodes")),
		FText::FromString(TEXT("Begin Node")),
		FText::FromString(TEXT("Make a new begin node")),
		0
	));
	TSharedPtr<FEventflowEdGraphSchemaAction> EndNodeAction = MakeShareable(new FEventflowEdGraphSchemaAction(
		UEventflowEdGraphEndNode::StaticClass(),
		FText::FromString(TEXT("Nodes")),
		FText::FromString(TEXT("End Node")),
		FText::FromString(TEXT("Make a new end node")),
		0
	));

	ContextMenuBuilder.AddAction(BeginNodeAction);
	ContextMenuBuilder.AddAction(EndNodeAction);
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

	/*if (A->PinType.PinCategory != B->PinType.PinCategory)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect pins of different category"));
	}*/

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
	NewNode->CreateDefaultPins();
	NewNode->NodePosX = Location.X;
	NewNode->NodePosY = Location.Y;
	NewNode->SetAssetNodeData(NewObject<UEventflowNodeData>(NewNode));

	// UEdGraphPin* InputPin = NewNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, TEXT("Input"), true);

	// Called when the mouse is still on hold & new node is created
	// usually during drag & drop of pins
	// if (FromPin)
	// {
	//		NewNode->GetSchema()->TryCreateConnection(FromPin, InputPin);
	// }

	ParentGraph->Modify();
	ParentGraph->AddNode(NewNode, true, true);

	return NewNode;
}
