// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdGraphNode.h"

// Engine Headers
#include "Framework/Commands/UIAction.h"
#include "ToolMenu.h"
#include "Algo/Reverse.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowNodeData.h"

#include "RenEventflowEditor/Public/Graph/EventflowEdGraphSchema.h"



FName UEventflowEdGraphNode::GetNodeType() const
{
	return FName(TEXT("REN.EF.NODE"));
}

FText UEventflowEdGraphNode::GetNodeDescription() const
{
	return FText::FromString(TEXT("Eventflow Node Description"));
}

TSubclassOf<UEventflowNodeData> UEventflowEdGraphNode::GetNodeDataClass() const
{
	return UEventflowNodeData::StaticClass();
}

bool UEventflowEdGraphNode::IsEntryNode() const
{
	return false;
}

void UEventflowEdGraphNode::SetNodeData(UEventflowNodeData* AssetNodeData)
{
	CachedNodeData = AssetNodeData;
}

UEventflowNodeData* UEventflowEdGraphNode::GetNodeData() const
{
	return CachedNodeData;
}

void UEventflowEdGraphNode::SyncPins()
{
	if (!CanCreateRuntimeInputPins() && !CanCreateRuntimeOutputPins()) return;

	UEventflowNodeData* NodeData = GetNodeData();
	if (!NodeData) return;

	TArray<UEdGraphPin*> InputLinks;
	TArray<UEdGraphPin*> OutputLinks;

	int TotalPins = Pins.Num();
	while (TotalPins > 0)
	{
		UEdGraphPin* Pin = GetPinAt(TotalPins - 1);
		if (!Pin->PinType.bIsConst)
		{
			if (Pin->Direction == EEdGraphPinDirection::EGPD_Input)
			{
				if (Pin->LinkedTo.IsValidIndex(0))
				{
					InputLinks.Add(Pin->LinkedTo[0]);
				}
				else
				{
					InputLinks.Add(nullptr);
				}
			}
			else
			{
				if (Pin->LinkedTo.IsValidIndex(0))
				{
					OutputLinks.Add(Pin->LinkedTo[0]);
				}
				else
				{
					OutputLinks.Add(nullptr);
				}
			}
			RemovePin(Pin);
		}
		TotalPins--;
	}

	Algo::Reverse(InputLinks);
	Algo::Reverse(OutputLinks);

	if (CanCreateRuntimeInputPins())
	{
		const TArray<FText>* InputOptions = NodeData->GetInputOptions();
		if (!InputOptions) return;

		CreatePinHelper(FText::FromString(TEXT("Input")), EEdGraphPinDirection::EGPD_Input, InputOptions, InputLinks);
		
	}

	if (CanCreateRuntimeOutputPins())
	{
		const TArray<FText>* OutputOptions = NodeData->GetOutputOptions();
		if (!OutputOptions) return;

		CreatePinHelper(FText::FromString(TEXT("Output")), EEdGraphPinDirection::EGPD_Output, OutputOptions, OutputLinks);

	}
}



bool UEventflowEdGraphNode::CanCreateRuntimeInputPins() const
{
	return false;
}

bool UEventflowEdGraphNode::CanCreateRuntimeOutputPins() const
{
	return false;
}

void UEventflowEdGraphNode::CreatePinHelper(FText FriendlyName, EEdGraphPinDirection Direction, const TArray<FText>* Options, const TArray<UEdGraphPin*>& CachedLinks)
{
	if (!Options) return;
	
	const UEdGraphSchema* Schema = GetSchema();

	int Count = Options->Num();
	for (int i = 0; i < Count; i++)
	{
		FText Option = (*Options)[i];

		FName PinName = FName(FGuid::NewGuid().ToString());
		UEdGraphPin* Pin = CreatePin(Direction, UEventflowEdGraphSchema::PC_Wildcard, PinName);
		if (!Pin) continue;

		Pin->PinFriendlyName = Option.IsEmpty() ? FriendlyName : FText::FromString(LimitTextLength(Option.ToString(), 10));
		Pin->PinType.bIsConst = false;

		if (CachedLinks.IsValidIndex(i))
		{
			if (CachedLinks[i])
			{
				Schema->TryCreateConnection(Pin, CachedLinks[i]);
				// Pin->MakeLinkTo(CachedLinks[i]);
			}
		}
	}
}

FString UEventflowEdGraphNode::LimitTextLength(const FString& InText, int MaxCharacters) const
{
	if (InText.Len() > MaxCharacters)
	{
		return InText.Left(MaxCharacters) + TEXT("...");
	}
	return InText;
}



FText UEventflowEdGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Eventflow Node"));
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

