// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "EdGraph/EdGraph.h"

// Generated Headers
#include "EventflowEdGraphSchemaAction.generated.h"

// Module Macros
#define REN_API RENEVENTFLOWED_API

// Forward Declarations
class UEventflowEdGraphNode;


/*
 *
 */
USTRUCT()
struct FEventflowEdGraphSchemaAction : public FEdGraphSchemaAction
{

	GENERATED_BODY()

public:

	FEventflowEdGraphSchemaAction();
	FEventflowEdGraphSchemaAction(TSubclassOf<UEventflowEdGraphNode> InNodeClass, FText InCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping);

	// ~ FEdGraphSchemaAction
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	// ~ End of FEdGraphSchemaAction

protected:

	TSubclassOf<UEventflowEdGraphNode> NodeClass = nullptr;

};


// Module Macros
#undef REN_API

