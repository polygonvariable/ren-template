// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "EdGraphUtilities.h"
#include "KismetPins/SGraphPinColor.h"
#include "EdGraph/EdGraphPin.h"

// Project Headers

// Generated Headers

// Forward Declarations



class SEventflowEdGraphPin : public SGraphPin
{

public:

	SLATE_BEGIN_ARGS(SEventflowEdGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

protected:

	virtual FSlateColor GetPinColor() const override;

};



class SEventflowEdGraphFlowPin : public SGraphPin
{

public:

	SLATE_BEGIN_ARGS(SEventflowEdGraphFlowPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

protected:

	virtual FSlateColor GetPinColor() const override;

};



struct FEventflowEdPanelPinFactory : public FGraphPanelPinFactory
{

public:

	virtual ~FEventflowEdPanelPinFactory();
	virtual TSharedPtr<SGraphPin> CreatePin(class UEdGraphPin* Pin) const override;

};

