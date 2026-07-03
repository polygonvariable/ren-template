// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "App/EventflowEdApp.h"

// Forward Declarations
class UEventflowAsset;
class UEventflowEdGraph;


/*
 *
 */
class FQuestEdApp : public FEventflowEdApp
{

public:

	// ~ UEventflowEdApp
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	// ~ End of UEventflowEdApp

protected:

	// ~ UEventflowEdApp
	virtual TSubclassOf<UEventflowEdGraphSchema> GetGraphSchemaClass() const override;
	virtual TArray<FName> GetTriggerNodeProperties() const override;
	// ~ End of UEventflowEdApp

};

