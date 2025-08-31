// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdTabFactory.h"

// Engine Headers
#include "GraphEditor.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEd/Public/App/EventflowEdApp.h"
#include "RenEventflowEd/Public/Graph/EventflowEdGraph.h"



FEventflowEdGraphTab::FEventflowEdGraphTab(TSharedPtr<FEventflowEdApp> InEventflowEdApp) : FWorkflowTabFactory(FName("EventflowEdGraphTab"), InEventflowEdApp)
{
	EventflowEdApp = InEventflowEdApp;

	TabLabel = FText::FromString(TEXT("Editor Graph"));
	ViewMenuDescription = FText::FromString(TEXT("Editor Graph"));
	ViewMenuTooltip = FText::FromString(TEXT("Editor Graph"));
}

TSharedRef<SWidget> FEventflowEdGraphTab::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FEventflowEdApp> App = EventflowEdApp.Pin();

	UEventflowEdGraph* WorkingGraph = App->GetWorkingGraph();

	SGraphEditor::FGraphEditorEvents GraphEvents;
	App->RegisterGraphEditorEvents(GraphEvents);

	TSharedPtr<SGraphEditor> GraphEditor =
		SNew(SGraphEditor)
			.IsEditable(true)
			.GraphEvents(GraphEvents)
			.GraphToEdit(WorkingGraph);

	App->SetGraphEditorSlate(GraphEditor);
	
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			GraphEditor.ToSharedRef()
		];
}



FEventflowEdGraphPropertyTab::FEventflowEdGraphPropertyTab(TSharedPtr<FEventflowEdApp> InEventflowEdApp) : FWorkflowTabFactory(FName("EventflowEdGraphPropertyTab"), InEventflowEdApp)
{
	EventflowEdApp = InEventflowEdApp;

	TabLabel = FText::FromString(TEXT("Graph Property"));
	ViewMenuDescription = FText::FromString(TEXT("Graph Property"));
	ViewMenuTooltip = FText::FromString(TEXT("Graph Property"));
}

TSharedRef<SWidget> FEventflowEdGraphPropertyTab::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FEventflowEdApp> App = EventflowEdApp.Pin();
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs Args;
	{
		Args.bAllowSearch = false;
		Args.bHideSelectionTip = true;
		Args.bLockable = false;
		Args.bSearchInitialKeyFocus = true;
		Args.bUpdatesFromSelection = false;
		Args.NotifyHook = nullptr;
		Args.bShowOptions = true;
		Args.bShowModifiedPropertiesOption = false;
		Args.bShowScrollBar = false;
	}

	TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(Args);
	DetailsView->SetObject(App->GetWorkingAsset());
	App->SetGraphPropertySlate(DetailsView);

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			DetailsView.ToSharedRef()
		];
}



FEventflowEdNodePropertyTab::FEventflowEdNodePropertyTab(TSharedPtr<FEventflowEdApp> InEventflowEdApp) : FWorkflowTabFactory(FName("EventflowEdNodePropertyTab"), InEventflowEdApp)
{
	EventflowEdApp = InEventflowEdApp;

	TabLabel = FText::FromString(TEXT("Node Property"));
	ViewMenuDescription = FText::FromString(TEXT("Node Property"));
	ViewMenuTooltip = FText::FromString(TEXT("Node Property"));
}

TSharedRef<SWidget> FEventflowEdNodePropertyTab::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FEventflowEdApp> App = EventflowEdApp.Pin();
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	FDetailsViewArgs Args;
	{
		Args.bAllowSearch = false;
		Args.bHideSelectionTip = true;
		Args.bLockable = false;
		Args.bSearchInitialKeyFocus = true;
		Args.bUpdatesFromSelection = false;
		Args.NotifyHook = nullptr;
		Args.bShowOptions = true;
		Args.bShowModifiedPropertiesOption = false;
		Args.bShowScrollBar = false;
	}

	TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(Args);
	DetailsView->SetObject(nullptr);
	App->SetNodePropertySlate(DetailsView);

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			DetailsView.ToSharedRef()
		];
}

