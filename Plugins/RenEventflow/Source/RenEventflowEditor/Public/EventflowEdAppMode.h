// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

// Project Headers

// Generated Headers

// Forward Declarations
class EventflowEdApp;


class EventflowEdAppMode : public FApplicationMode
{

public:

	EventflowEdAppMode(TSharedPtr<EventflowEdApp> GraphEditorApp);

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

private:

	TWeakPtr<EventflowEdApp> GraphEditorAppPtr;
	FWorkflowAllowedTabSet AllowedTabSet;

};