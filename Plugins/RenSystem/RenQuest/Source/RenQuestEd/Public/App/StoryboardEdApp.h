// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "BlueprintEditor.h"
#include "BlueprintEditorModes.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

// Project Headers
#include "RenEventflowEd/Public/App/EventflowEdApp.h"

// Generated Headers

// Forward Declarations
class UEventflowEdGraphSchema;



class FStoryboardEdApp : public FEventflowEdApp
{

public:

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

protected:

	virtual void OnGraphNodeDoubleClicked(UEdGraphNode* Node) override;

	virtual TSubclassOf<UEventflowEdGraphSchema> GetGraphSchemaClass() const override;
	virtual TArray<FName> GetTriggerNodeProperties() const override;


};

