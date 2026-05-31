// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "EdGraphUtilities.h"
#include "SGraphPin.h"


/*
 * 
 */
class SEventflowEdGraphPin : public SGraphPin
{

public:

	SLATE_BEGIN_ARGS(SEventflowEdGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

protected:

	// ~ SGraphPin
	virtual FSlateColor GetPinColor() const override;
	virtual const FSlateBrush* GetPinIcon() const override;
	// ~ End of SGraphPin

};


/*
 *
 */
class SEventflowEdGraphFlowPin : public SGraphPin
{

public:

	SLATE_BEGIN_ARGS(SEventflowEdGraphFlowPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

protected:

	// ~ SGraphPin
	virtual FSlateColor GetPinColor() const override;
	virtual const FSlateBrush* GetPinIcon() const override;
	// ~ End of SGraphPin

};


/*
 *
 */
struct FEventflowEdPanelPinFactory : public FGraphPanelPinFactory
{

public:

	// ~ FGraphPanelPinFactory
	virtual ~FEventflowEdPanelPinFactory();
	virtual TSharedPtr<SGraphPin> CreatePin(class UEdGraphPin* Pin) const override;
	// ~ End of FGraphPanelPinFactory

};

