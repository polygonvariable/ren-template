// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflowEd/Public/Graph/EventflowEdGraphSchema.h"

// Generated Headers
#include "StoryboardEdGraphSchema.generated.h"

// Forward Declarations



UCLASS()
class UStoryboardEdGraphSchema : public UEventflowEdGraphSchema
{

	GENERATED_BODY()

public:

	virtual TArray<UClass*> GetNodeClasses() const override;

	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;

};

