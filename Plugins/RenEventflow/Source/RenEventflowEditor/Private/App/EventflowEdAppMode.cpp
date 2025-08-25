// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "App/EventflowEdAppMode.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEditor/Public/App/EventflowEdApp.h"
#include "RenEventflowEditor/Public/Graph/EventflowEdTabFactory.h"



FEventflowEdAppMode::FEventflowEdAppMode(TSharedPtr<FEventflowEdApp> InEventflowEdApp) : FApplicationMode(TEXT("RGraphEditorAppMode"))
{
	AllowedTabSet.RegisterFactory(MakeShareable(new FEventflowEdTabFactory(InEventflowEdApp)));
	AllowedTabSet.RegisterFactory(MakeShareable(new FEventflowEdPropertyTabFactory(InEventflowEdApp)));

	EventflowEdApp = InEventflowEdApp;

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

void FEventflowEdAppMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<FEventflowEdApp> App = EventflowEdApp.Pin();
	App->PushTabFactories(AllowedTabSet);

	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FEventflowEdAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void FEventflowEdAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
