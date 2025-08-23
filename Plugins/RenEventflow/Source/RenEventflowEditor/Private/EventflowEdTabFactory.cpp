// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEdTabFactory.h"

// Engine Headers
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "GraphEditor.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEditor/Public/EventflowEdApp.h"


EventflowEdTabFactory::EventflowEdTabFactory(TSharedPtr<EventflowEdApp> GraphEditorApp) : FWorkflowTabFactory(FName("RGraphPrimaryTab"), GraphEditorApp)
{
	GraphEditorAppPtr = GraphEditorApp;
	TabLabel = FText::FromString(TEXT("Primary Tab"));
	ViewMenuDescription = FText::FromString(TEXT("Primary Tab Menu Description"));
	ViewMenuTooltip = FText::FromString(TEXT("Primary Tab Menu Tooltip"));
}

TSharedRef<SWidget> EventflowEdTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<EventflowEdApp> App = GraphEditorAppPtr.Pin();

	SGraphEditor::FGraphEditorEvents GraphEditorEvents;
	GraphEditorEvents.OnSelectionChanged.BindRaw(App.Get(), &EventflowEdApp::OnGraphSelectionChanged);

	TSharedPtr<SGraphEditor> GraphEditor =
		SNew(SGraphEditor)
			.IsEditable(true)
			.GraphEvents(GraphEditorEvents)
			.GraphToEdit(App->GetWorkingGraph());

	App->SetWorkingGraphEditor(GraphEditor);

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			GraphEditor.ToSharedRef()
		];
}

FText EventflowEdTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("Primary Tab Tool Tip"));
}



EventflowEdPropertyTabFactory::EventflowEdPropertyTabFactory(TSharedPtr<EventflowEdApp> GraphEditorApp) : FWorkflowTabFactory(FName("RGraphPropertyTab"), GraphEditorApp)
{
	GraphEditorAppPtr = GraphEditorApp;
	TabLabel = FText::FromString(TEXT("Property Tab"));
	ViewMenuDescription = FText::FromString(TEXT("Property Tab Menu Description"));
	ViewMenuTooltip = FText::FromString(TEXT("Property Tab Menu Tooltip"));
}

TSharedRef<SWidget> EventflowEdPropertyTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<EventflowEdApp> GraphEditorApp = GraphEditorAppPtr.Pin();
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

FText EventflowEdPropertyTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("Primary Tab Tool Tip"));
}
