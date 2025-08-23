// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEdGraphNode.h"

// Engine Headers
#include "Framework/Commands/UIAction.h"
#include "ToolMenu.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEditor/Public/EventflowEdApp.h"



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

//UEdGraphPin* UEventflowEdGraphNode::CreateCustomPin(EEdGraphPinDirection Direction, FName Title, bool bIsConst)
//{
//	FName Catergory = (Direction == EEdGraphPinDirection::EGPD_Output) ? TEXT("Output") : TEXT("Input");
//	FName SubCatergory = TEXT("CUSTOM_PIN");
//
//	UEdGraphPin* Pin = CreatePin(
//		Direction,
//		Catergory,
//		Title
//	);
//	Pin->PinType.PinSubCategory = SubCatergory;
//	Pin->PinType.bIsConst = bIsConst;
//	return Pin;
//}

UEdGraphPin* UEventflowEdGraphNode::CreatePinHelper(EEdGraphPinDirection Direction, FName Title, FName Catergory, bool bIsConst)
{
	UEdGraphPin* Pin = CreatePin(Direction, Catergory, Title);
	Pin->PinType.bIsConst = bIsConst;

	return Pin;
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

void UEventflowEdGraphNode::SyncOutputPins()
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

	for (FText Option : AssetNodeData->InputOptions)
	{
		UEdGraphPin* Pin = CreatePinHelper(EEdGraphPinDirection::EGPD_Input, FName(Option.ToString()), TEXT("CUSTOM_PIN"), false);
		Pin->PinName = FName(FGuid::NewGuid().ToString());
		Pin->PinFriendlyName = FText::FromString(Option.ToString());
	}

	for (FText Option : AssetNodeData->OutputOptions)
	{
		UEdGraphPin* Pin = CreatePinHelper(EEdGraphPinDirection::EGPD_Output, FName(Option.ToString()), TEXT("CUSTOM_PIN"), false);
		Pin->PinName = FName(FGuid::NewGuid().ToString());
		Pin->PinFriendlyName = FText::FromString(Option.ToString());
	}
}



void UEventflowEdGraphBeginNode::CreateDefaultPins()
{
	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Output, TEXT("Output"), TEXT("Exec"));
	Pin->PinFriendlyName = FText::FromString(TEXT("Exec"));
	Pin->PinType.bIsConst = true;
}

void UEventflowEdGraphEndNode::CreateDefaultPins()
{
	UEdGraphPin* Pin = CreatePin(EEdGraphPinDirection::EGPD_Input, TEXT("Input"), TEXT("Return"));
	Pin->PinFriendlyName = FText::FromString(TEXT("Return"));
	Pin->PinType.bIsConst = true;
}

