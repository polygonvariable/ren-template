// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflowEd/Public/App/EventflowEdApp.h"

// Generated Headers

// Forward Declarations
class UEventflowEdGraphSchema;



class FStoryboardEdApp : public FEventflowEdApp
{

protected:

	virtual TSubclassOf<UEventflowEdGraphSchema> GetGraphSchemaClass() const override;

public:

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	virtual void OnGraphNodeDoubleClicked(UEdGraphNode* Node) override;

};

