// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdGraphPin.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEditor/Public/App/EventflowEdApp.h"



void SEventflowEdGraphPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
}

FSlateColor SEventflowEdGraphPin::GetPinColor() const
{
	return FSlateColor(FLinearColor::White);
}

void SEventflowEdGraphFlowPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
}

FSlateColor SEventflowEdGraphFlowPin::GetPinColor() const
{
	return FSlateColor(FLinearColor::Red);
}




FEventflowEdPanelPinFactory::~FEventflowEdPanelPinFactory()
{
}

TSharedPtr<SGraphPin> FEventflowEdPanelPinFactory::CreatePin(UEdGraphPin* Pin) const
{
	if (Pin->PinType.bIsConst)
	{
		return SNew(SEventflowEdGraphFlowPin, Pin);
	}
	return nullptr;

	/*if (Pin->PinType.PinCategory == FName(TEXT("CUSTOM_PIN")))
	{
		return SNew(SEventflowEdGraphPin, Pin);
	}
	if (Pin->PinType.PinCategory == FName(TEXT("BASE_PIN")))
	{
		return SNew(SEventflowEdGraphPin, Pin);
	}
	if (Pin->PinType.PinCategory == FName(TEXT("FLOW_PIN")))
	{
		return SNew(SEventflowEdGraphFlowPin, Pin);
	}
	return nullptr;*/
}

