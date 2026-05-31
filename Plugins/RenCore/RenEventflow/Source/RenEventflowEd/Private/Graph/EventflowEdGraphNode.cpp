// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdGraphNode.h"

// Engine Headers
#include "Algo/Reverse.h"
#include "EdGraph/EdGraphPin.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenu.h"

// Project Headers
#include "Graph/EventflowEdGraphSchema.h"


bool UEventflowEdGraphNode::GetIsEntryNode() const
{
	int PinCount = Pins.Num();
	int InputPinCount = 0;
	for (int i = 0; i < PinCount; i++)
	{
		UEdGraphPin* Pin = GetPinAt(i);
		if (Pin->Direction == EEdGraphPinDirection::EGPD_Input)
		{
			InputPinCount++;
		}
	}
	return InputPinCount == 0;
}

FText UEventflowEdGraphNode::GetNodeDescription() const
{
	return FText::FromString(TEXT("Eventflow Node Description"));
}


TArray<FText> UEventflowEdGraphNode::GetRuntimeInputPins() const
{
	return TArray<FText>();
}

TArray<FText> UEventflowEdGraphNode::GetRuntimeOutputPins() const
{
	return TArray<FText>();
}

void UEventflowEdGraphNode::SyncRuntimePins()
{
	TArray<TPair<FString, TArray<UEdGraphPin*>>> InputLinks;
	TArray<TPair<FString, TArray<UEdGraphPin*>>> OutputLinks;

	int TotalPins = Pins.Num();
	while (TotalPins > 0)
	{
		UEdGraphPin* Pin = GetPinAt(TotalPins - 1);
		if (!Pin->PinType.bIsConst)
		{
			FString FuzzyName = Pin->PinName.ToString();

			if (Pin->Direction == EEdGraphPinDirection::EGPD_Input)
			{
				InputLinks.Add(TPair<FString, TArray<UEdGraphPin*>>(FuzzyName, Pin->LinkedTo));
			}
			else
			{
				OutputLinks.Add(TPair<FString, TArray<UEdGraphPin*>>(FuzzyName, Pin->LinkedTo));
			}
			RemovePin(Pin);
		}
		TotalPins--;
	}

	Algo::Reverse(InputLinks);
	Algo::Reverse(OutputLinks);

	const TArray<FText>& InputPins = GetRuntimeInputPins();
	const TArray<FText>& OutputPins = GetRuntimeOutputPins();

	CreateRuntimePins(InputPins, EEdGraphPinDirection::EGPD_Input);
	CreateRuntimePins(OutputPins, EEdGraphPinDirection::EGPD_Output);

	FuzzyMatchRuntimePins(InputLinks);
	FuzzyMatchRuntimePins(OutputLinks);
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


void UEventflowEdGraphNode::CreateRuntimePins(const TArray<FText>& PinNames, EEdGraphPinDirection Direction)
{
	const UEdGraphSchema* Schema = GetSchema();
	for (const FText& PinName : PinNames)
	{
		FName SafePinName = CreateUniquePinName(FName(*PinName.ToString()));
		UEdGraphPin* Pin = CreatePin(Direction, UEventflowEdGraphSchema::PC_Exec, SafePinName);
		if (!Pin)
		{
			continue;
		}
		Pin->PinId = FGuid::NewGuid();
		Pin->PinFriendlyName = FText::FromName(SafePinName);
		Pin->PinType.bIsConst = false;
	}
}

void UEventflowEdGraphNode::FuzzyMatchRuntimePins(const TArray<TPair<FString, TArray<UEdGraphPin*>>> FuzzyPins)
{
	for (const TPair<FString, TArray<UEdGraphPin*>>& Pair : FuzzyPins)
	{
		UEdGraphPin* Pin = FindPin(Pair.Key);
		if (!Pin)
		{
			continue;
		}

		const TArray<UEdGraphPin*>& LinkedPins = Pair.Value;
		for (UEdGraphPin* LinkedPin : LinkedPins)
		{
			if (LinkedPin)
			{
				Pin->MakeLinkTo(LinkedPin);
			}
		}
	}
}

