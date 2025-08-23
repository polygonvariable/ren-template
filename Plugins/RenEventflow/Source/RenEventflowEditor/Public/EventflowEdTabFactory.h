// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

// Project Headers

// Generated Headers

// Forward Declarations
class EventflowEdApp;
class SWidget;


class EventflowEdTabFactory : public FWorkflowTabFactory
{

public:

	EventflowEdTabFactory(TSharedPtr<EventflowEdApp> GraphEditorApp);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:

	TWeakPtr<EventflowEdApp> GraphEditorAppPtr;

};

class EventflowEdPropertyTabFactory : public FWorkflowTabFactory
{

public:

	EventflowEdPropertyTabFactory(TSharedPtr<EventflowEdApp> GraphEditorApp);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:

	TWeakPtr<EventflowEdApp> GraphEditorAppPtr;

};

