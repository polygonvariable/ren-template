// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

// Forward Declarations
class SWidget;
class FEventflowEdApp;


/*
 *
 */
class FEventflowEdGraphTab : public FWorkflowTabFactory
{

public:

	FEventflowEdGraphTab(TSharedPtr<FEventflowEdApp> App);

	// ~ FWorkflowTabFactory
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	// ~ End of FWorkflowTabFactory

};


/*
 *
 */
class FEventflowEdGraphPropertyTab : public FWorkflowTabFactory
{

public:

	FEventflowEdGraphPropertyTab(TSharedPtr<FEventflowEdApp> App);

	// ~ FWorkflowTabFactory
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	// ~ End of FWorkflowTabFactory

protected:

	int GetNumberOfObjects() const;

};


/*
 *
 */
class FEventflowEdNodePropertyTab : public FWorkflowTabFactory
{

public:

	FEventflowEdNodePropertyTab(TSharedPtr<FEventflowEdApp> App);

	// ~ FWorkflowTabFactory
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	// ~ End of FWorkflowTabFactory

};

