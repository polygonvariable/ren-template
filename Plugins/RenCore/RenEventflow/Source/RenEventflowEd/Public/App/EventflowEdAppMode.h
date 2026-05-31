// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

// Forward Declarations
class FEventflowEdApp;


/*
 * 
 */
class FEventflowEdAppMode : public FApplicationMode
{

public:

	FEventflowEdAppMode(TSharedPtr<FEventflowEdApp> InEventflowEdApp);

	// ~ FApplicationMode
	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	// ~ End of FApplicationMode

private:

	TWeakPtr<FEventflowEdApp> EventflowEdApp;
	FWorkflowAllowedTabSet EventflowEdTabSet;

};

