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



class FEventflowEdGraphTab : public FWorkflowTabFactory
{

public:

	FEventflowEdGraphTab(TSharedPtr<FEventflowEdApp> InEventflowEdApp);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;

private:

	TWeakPtr<FEventflowEdApp> EventflowEdApp;

};



class FEventflowEdGraphPropertyTab : public FWorkflowTabFactory
{

public:

	FEventflowEdGraphPropertyTab(TSharedPtr<FEventflowEdApp> InEventflowEdApp);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;

private:

	TWeakPtr<FEventflowEdApp> EventflowEdApp;

};




class FEventflowEdNodePropertyTab : public FWorkflowTabFactory
{

public:

	FEventflowEdNodePropertyTab(TSharedPtr<FEventflowEdApp> InEventflowEdApp);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;

private:

	TWeakPtr<FEventflowEdApp> EventflowEdApp;

};

