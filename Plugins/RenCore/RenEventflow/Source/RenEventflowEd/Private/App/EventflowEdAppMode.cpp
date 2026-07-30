// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "App/EventflowEdAppMode.h"

// Project Headers
#include "App/EventflowEdApp.h"
#include "Graph/EventflowEdTabFactory.h"


FEventflowEdAppMode::FEventflowEdAppMode(TSharedPtr<FEventflowEdApp> InEventflowEdApp) : FApplicationMode(TEXT("RGraphEditorAppMode"))
{
	EventflowEdApp = InEventflowEdApp;

	EventflowEdTabSet.RegisterFactory(MakeShareable(new FEventflowEdGraphTab(InEventflowEdApp)));
	EventflowEdTabSet.RegisterFactory(MakeShareable(new FEventflowEdNodePropertyTab(InEventflowEdApp)));
	EventflowEdTabSet.RegisterFactory(MakeShareable(new FEventflowEdGraphPropertyTab(InEventflowEdApp)));

	TabLayout = FTabManager::NewLayout("RGraphEditorAppLayout_v1")
		->AddArea(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->Split(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.25)
					->AddTab(FName(TEXT("EventflowEdGraphPropertyTab")), ETabState::OpenedTab)
				)
				->Split(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.5)
					->AddTab(FName(TEXT("EventflowEdGraphTab")), ETabState::OpenedTab)
				)
				->Split(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.25)
					->AddTab(FName(TEXT("EventflowEdNodePropertyTab")), ETabState::OpenedTab)
				)
			)
		);
}

void FEventflowEdAppMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<FEventflowEdApp> App = EventflowEdApp.Pin();
	App->PushTabFactories(EventflowEdTabSet);
	
	FApplicationMode::RegisterTabFactoriesWithManager(InTabManager);
}

