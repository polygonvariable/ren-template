// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdGraphPin.h"

// Engine Headers

// Project Headers
#include "RenEventflowEd/Public/Graph/EventflowEdGraphSchema.h"



void SEventflowEdGraphPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
}

FSlateColor SEventflowEdGraphPin::GetPinColor() const
{
	return FSlateColor(FLinearColor(0.0f, 1.0f, 1.0f));
}



void SEventflowEdGraphFlowPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
}

FSlateColor SEventflowEdGraphFlowPin::GetPinColor() const
{
	return FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f));
}



FEventflowEdPanelPinFactory::~FEventflowEdPanelPinFactory()
{
}

TSharedPtr<SGraphPin> FEventflowEdPanelPinFactory::CreatePin(UEdGraphPin* Pin) const
{
	if (Pin->PinType.PinCategory == UEventflowEdGraphSchema::PC_Exec)
	{
		return SNew(SEventflowEdGraphFlowPin, Pin);
	}
	else if (Pin->PinType.PinCategory == UEventflowEdGraphSchema::PC_Wildcard)
	{
		return SNew(SEventflowEdGraphPin, Pin);
	}
	return nullptr;
}

