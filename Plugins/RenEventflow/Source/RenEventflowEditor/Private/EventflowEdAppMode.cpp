// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEdAppMode.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEditor/Public/EventflowEdApp.h"
#include "RenEventflowEditor/Public/EventflowEdTabFactory.h"



EventflowEdAppMode::EventflowEdAppMode(TSharedPtr<EventflowEdApp> GraphEditorApp) : FApplicationMode(TEXT("RGraphEditorAppMode"))
{
	AllowedTabSet.RegisterFactory(MakeShareable(new EventflowEdTabFactory(GraphEditorApp)));
	AllowedTabSet.RegisterFactory(MakeShareable(new EventflowEdPropertyTabFactory(GraphEditorApp)));

	GraphEditorAppPtr = GraphEditorApp;

	TabLayout = FTabManager::NewLayout("RGraphEditorAppLayout_v1")
		->AddArea(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->Split(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.75)
					->AddTab(FName(TEXT("RGraphPrimaryTab")), ETabState::OpenedTab)
				)
				->Split(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.25)
					->AddTab(FName(TEXT("RGraphPropertyTab")), ETabState::OpenedTab)
				)
			)
		);
}

void EventflowEdAppMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<EventflowEdApp> App = GraphEditorAppPtr.Pin();
	App->PushTabFactories(AllowedTabSet);

	FApplicationMode::RegisterTabFactories(InTabManager);
}

void EventflowEdAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void EventflowEdAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
