// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

// Project Headers

// Generated Headers

// Forward Declarations
class SWidget;

class FEventflowEdApp;



class FEventflowEdGraphTabFactory : public FWorkflowTabFactory
{

public:

	FEventflowEdGraphTabFactory(TSharedPtr<FEventflowEdApp> GraphEditorApp);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:

	TWeakPtr<FEventflowEdApp> GraphEditorAppPtr;

};



class FEventflowEdPropertyTabFactory : public FWorkflowTabFactory
{

public:

	FEventflowEdPropertyTabFactory(TSharedPtr<FEventflowEdApp> GraphEditorApp);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:

	TWeakPtr<FEventflowEdApp> GraphEditorAppPtr;

};

