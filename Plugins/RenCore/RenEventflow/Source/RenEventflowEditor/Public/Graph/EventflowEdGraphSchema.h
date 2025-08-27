// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"

// Project Headers

// Generated Headers
#include "EventflowEdGraphSchema.generated.h"

// Forward Declarations
class UEventflowNodeData;

class UEventflowEdGraphNode;



UCLASS()
class RENEVENTFLOWEDITOR_API UEventflowEdGraphSchema : public UEdGraphSchema
{

	GENERATED_BODY()

public:

	static const FName PC_Exec;
	static const FName PC_Wildcard;

	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;

protected:

	virtual void AddGraphNodeActions(FGraphContextMenuBuilder& ContextMenuBuilder, TSubclassOf<UEventflowEdGraphNode> NodeClass, const FString& InCategory, FText InMenuDesc, FText InToolTip) const;

};



USTRUCT()
struct RENEVENTFLOWEDITOR_API FEventflowEdGraphSchemaAction : public FEdGraphSchemaAction
{

	GENERATED_BODY()

public:

	FEventflowEdGraphSchemaAction();
	FEventflowEdGraphSchemaAction(TSubclassOf<UEventflowEdGraphNode> InNodeClass, FText InCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping);

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true);

protected:

	TSubclassOf<UEventflowEdGraphNode> NodeClass;

	virtual TSubclassOf<UEventflowNodeData> GetAssetNodeDataClass() const;

};

