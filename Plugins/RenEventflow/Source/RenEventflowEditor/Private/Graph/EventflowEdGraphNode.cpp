// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdGraphNode.h"

// Engine Headers
#include "Framework/Commands/UIAction.h"
#include "ToolMenu.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEditor/Public/App/EventflowEdApp.h"



FText UEventflowEdGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Base Node"));
}

FLinearColor UEventflowEdGraphNode::GetNodeTitleColor() const
{
	return FLinearColor::White;
}

bool UEventflowEdGraphNode::CanUserDeleteNode() const
{
	return true;
}

void UEventflowEdGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection("SectionName", FText::FromString("Action Name"));

	UEventflowEdGraphNode* Node = (UEventflowEdGraphNode*)this;

	Section.AddMenuEntry(
		TEXT("DELETE_NODE"),
		FText::FromString(TEXT("Delete Node")),
		FText::FromString(TEXT("Delete a node")),
		FSlateIcon(TEXT("GraphEditorStyle"), TEXT("Icons.Pin")),
		FUIAction(FExecuteAction::CreateLambda(
			[Node]()
			{
				Node->GetGraph()->RemoveNode(Node);
			}
		))
	);
}


bool UEventflowEdGraphNode::CanCreateRuntimeInputPins() const
{
	return false;
}

bool UEventflowEdGraphNode::CanCreateRuntimeOutputPins() const
{
	return false;
}



void UEventflowEdGraphNode::SetAssetNodeData(UEventflowNodeData* AssetNodeData)
{
	CachedAssetNodeData = AssetNodeData;
}

UEventflowNodeData* UEventflowEdGraphNode::GetAssetNodeData() const
{
	return CachedAssetNodeData;
}

void UEventflowEdGraphNode::CreateDefaultPins()
{
}

void UEventflowEdGraphNode::SyncPins()
{
	UEventflowNodeData* AssetNodeData = GetAssetNodeData();
	if (!AssetNodeData) return;

	int TotalPins = Pins.Num();
	while (TotalPins > 0)
	{
		UEdGraphPin* Pin = GetPinAt(TotalPins - 1);
		if (!Pin->PinType.bIsConst)
		{
			RemovePin(Pin);
		}
		TotalPins--;
	}

	if (CanCreateRuntimeInputPins())
	{
		for (FText Option : AssetNodeData->InputOptions)
		{
			FName PinName = Option.IsEmpty() ? FName(TEXT("Input")) : FName(FGuid::NewGuid().ToString());
			UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Input, TEXT("In"), PinName);
			if (!Pin) continue;

			Pin->PinFriendlyName = Option.IsEmpty() ? FText::FromString(TEXT("Input")) : FText::FromString(Option.ToString());
			Pin->PinType.bIsConst = false;
		}
	}

	if (CanCreateRuntimeOutputPins())
	{
		for (FText Option : AssetNodeData->OutputOptions)
		{
			FName PinName = Option.IsEmpty() ? FName(TEXT("Output")) : FName(FGuid::NewGuid().ToString());
			UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Output, TEXT("Out"), PinName);
			if (!Pin) continue;

			Pin->PinFriendlyName = Option.IsEmpty() ? FText::FromString(TEXT("Output")) : FText::FromString(Option.ToString());
			Pin->PinType.bIsConst = false;
		}
	}
}



void UEventflowEdGraphBeginNode::CreateDefaultPins()
{
	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Output, TEXT("Exec"), TEXT("Exec"));
	Pin->PinFriendlyName = FText::FromString(TEXT("Exec"));
	Pin->PinType.bIsConst = true;
}

void UEventflowEdGraphEndNode::CreateDefaultPins()
{
	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Input, TEXT("Exec"), TEXT("Return"));
	Pin->PinFriendlyName = FText::FromString(TEXT("Return"));
	Pin->PinType.bIsConst = true;
}

