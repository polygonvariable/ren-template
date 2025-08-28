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

	virtual TArray<UClass*> GetNodeClasses() const override;

	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;

};

