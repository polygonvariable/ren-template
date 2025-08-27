// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdTabFactory.h"

// Engine Headers
#include "GraphEditor.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEditor/Public/App/EventflowEdApp.h"
#include "RenEventflowEditor/Public/Graph/EventflowEdGraph.h"



FEventflowEdGraphTabFactory::FEventflowEdGraphTabFactory(TSharedPtr<FEventflowEdApp> GraphEditorApp) : FWorkflowTabFactory(FName("RGraphPrimaryTab"), GraphEditorApp)
{
	GraphEditorAppPtr = GraphEditorApp;
	TabLabel = FText::FromString(TEXT("Eventflow Tab"));
	ViewMenuDescription = FText::FromString(TEXT("Primary Tab Menu Description"));
	ViewMenuTooltip = FText::FromString(TEXT("Primary Tab Menu Tooltip"));
}

TSharedRef<SWidget> FEventflowEdGraphTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FEventflowEdApp> App = GraphEditorAppPtr.Pin();

	SGraphEditor::FGraphEditorEvents GraphEditorEvents;
	GraphEditorEvents.OnSelectionChanged.BindRaw(App.Get(), &FEventflowEdApp::OnGraphSelectionChanged);

	UEventflowEdGraph* WorkingGraph = App->GetWorkingGraph();

	TSharedPtr<SGraphEditor> GraphEditor =
		SNew(SGraphEditor)
			.IsEditable(true)
			.GraphEvents(GraphEditorEvents)
			.GraphToEdit(WorkingGraph);

	App->SetWorkingGraphEditor(GraphEditor);

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			GraphEditor.ToSharedRef()
		];
}

FText FEventflowEdGraphTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("Primary Tab Tool Tip"));
}



FEventflowEdPropertyTabFactory::FEventflowEdPropertyTabFactory(TSharedPtr<FEventflowEdApp> GraphEditorApp) : FWorkflowTabFactory(FName("RGraphPropertyTab"), GraphEditorApp)
{
	GraphEditorAppPtr = GraphEditorApp;
	TabLabel = FText::FromString(TEXT("Property Tab"));
	ViewMenuDescription = FText::FromString(TEXT("Property Tab Menu Description"));
	ViewMenuTooltip = FText::FromString(TEXT("Property Tab Menu Tooltip"));
}

TSharedRef<SWidget> FEventflowEdPropertyTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FEventflowEdApp> GraphEditorApp = GraphEditorAppPtr.Pin();
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
	DetailsView->SetObject(GraphEditorApp->GetWorkingAsset());

	TSharedPtr<IDetailsView> SelectedNodeDetail = PropertyEditorModule.CreateDetailView(Args);
	SelectedNodeDetail->SetObject(nullptr);

	GraphEditorApp->SetSelectedNodeDetail(SelectedNodeDetail);

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			DetailsView.ToSharedRef()
		]
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			SelectedNodeDetail.ToSharedRef()
		];
}

FText FEventflowEdPropertyTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("Primary Tab Tool Tip"));
}

