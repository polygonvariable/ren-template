// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdTabFactory.h"

// Engine Headers
#include "GraphEditor.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

// Project Headers
#include "App/EventflowEdApp.h"
#include "Asset/EventflowAsset.h"
#include "Graph/EventflowEdGraph.h"


FEventflowEdGraphTab::FEventflowEdGraphTab(TSharedPtr<FEventflowEdApp> App) : FWorkflowTabFactory(FName("EventflowEdGraphTab"), App)
{
	HostingApp = App;
	
	TabLabel = FText::FromString(TEXT("Editor Graph"));
	ViewMenuDescription = FText::FromString(TEXT("Editor Graph"));
	ViewMenuTooltip = FText::FromString(TEXT("Editor Graph"));
}

TSharedRef<SWidget> FEventflowEdGraphTab::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FEventflowEdApp> App = StaticCastSharedPtr<FEventflowEdApp>(HostingApp.Pin());
	UEventflowEdGraph* WorkingGraph = App->GetWorkingGraph();

	SGraphEditor::FGraphEditorEvents GraphEvents;
	App->RegisterGraphEvent(GraphEvents);

	TSharedPtr<SGraphEditor> GraphEditor =
		SNew(SGraphEditor)
			.IsEditable(true)
			.GraphEvents(GraphEvents)
			.GraphToEdit(WorkingGraph);
	
	App->RegisterGraphEditor(GraphEditor);

	return
		SNew(SVerticalBox)
			+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				.HAlign(HAlign_Fill)
				[
					GraphEditor.ToSharedRef()
				];
}


FEventflowEdGraphPropertyTab::FEventflowEdGraphPropertyTab(TSharedPtr<FEventflowEdApp> App) : FWorkflowTabFactory(FName("EventflowEdGraphPropertyTab"), App)
{
	HostingApp = App;

	TabLabel = FText::FromString(TEXT("Graph Property"));
	ViewMenuDescription = FText::FromString(TEXT("Graph Property"));
	ViewMenuTooltip = FText::FromString(TEXT("Graph Property"));
}

TSharedRef<SWidget> FEventflowEdGraphPropertyTab::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FEventflowEdApp> App = StaticCastSharedPtr<FEventflowEdApp>(HostingApp.Pin());
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
	App->RegisterGraphProperty(DetailsView);

	return
		SNew(SVerticalBox)
			+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				.HAlign(HAlign_Fill)
				[
					DetailsView.ToSharedRef()
				]
			+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(4.0f)
				.HAlign(HAlign_Fill)
				[
					SNew(STextBlock)
						.Text(FText::FromString(FString::Printf(TEXT("Number of unique objects: %d"), GetNumberOfObjects())))
				];
}

int FEventflowEdGraphPropertyTab::GetNumberOfObjects() const
{
	TSharedPtr<FEventflowEdApp> App = StaticCastSharedPtr<FEventflowEdApp>(HostingApp.Pin());

	int ObjectCount = 0;
	ForEachObjectWithOuter(App->GetWorkingAsset(),
		[&ObjectCount](UObject* InnerObject)
		{
			++ObjectCount;
		},
		true
	);

	return ObjectCount;
}


FEventflowEdNodePropertyTab::FEventflowEdNodePropertyTab(TSharedPtr<FEventflowEdApp> App) : FWorkflowTabFactory(FName("EventflowEdNodePropertyTab"), App)
{
	HostingApp = App;

	TabLabel = FText::FromString(TEXT("Node Property"));
	ViewMenuDescription = FText::FromString(TEXT("Node Property"));
	ViewMenuTooltip = FText::FromString(TEXT("Node Property"));
}

TSharedRef<SWidget> FEventflowEdNodePropertyTab::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FEventflowEdApp> App = StaticCastSharedPtr<FEventflowEdApp>(HostingApp.Pin());
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
	App->RegisterNodeProperty(DetailsView);

	return
		SNew(SVerticalBox)
			+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				.HAlign(HAlign_Fill)
				[
					DetailsView.ToSharedRef()
				];
}

