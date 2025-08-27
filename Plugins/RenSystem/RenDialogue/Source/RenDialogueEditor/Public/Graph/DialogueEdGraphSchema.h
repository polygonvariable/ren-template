// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflowEditor/Public/Graph/EventflowEdGraphSchema.h"

// Generated Headers
#include "DialogueEdGraphSchema.generated.h"

// Forward Declarations
class UEventflowNodeData;



UCLASS()
class UDialogueEdGraphSchema : public UEventflowEdGraphSchema
{

	GENERATED_BODY()

public:

	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;

protected:

	virtual void AddGraphNodeActions(FGraphContextMenuBuilder& ContextMenuBuilder, TSubclassOf<UEventflowEdGraphNode> NodeClass, const FString& InCategory, FText InMenuDesc, FText InToolTip) const override;

};



USTRUCT()
struct FDialogueEdGraphSchemaAction : public FEventflowEdGraphSchemaAction
{

	GENERATED_BODY()

public:

	FDialogueEdGraphSchemaAction();
	FDialogueEdGraphSchemaAction(TSubclassOf<UEventflowEdGraphNode> InNodeClass, FText InCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping);

protected:

	virtual TSubclassOf<UEventflowNodeData> GetAssetNodeDataClass() const;

};

