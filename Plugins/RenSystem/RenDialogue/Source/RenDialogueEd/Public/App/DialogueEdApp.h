// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflowEd/Public/App/EventflowEdApp.h"

// Generated Headers

// Forward Declarations
class UEventflowAsset;

class UEventflowEdGraph;
class UEventflowEdGraphNode;
class UEventflowEdGraphSchema;



class FDialogueEdApp : public FEventflowEdApp
{
public:

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

protected:

	virtual TSubclassOf<UEventflowEdGraphSchema> GetGraphSchemaClass() const override;
	virtual TArray<FName> GetTriggerNodeProperties() const override;


};

