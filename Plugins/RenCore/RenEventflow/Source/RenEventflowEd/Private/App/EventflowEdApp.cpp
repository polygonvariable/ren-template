// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "App/EventflowEdApp.h"

// Engine Headers
#include "GraphEditor.h"
#include "Kismet2/BlueprintEditorUtils.h"

// Project Headers
#include "App/EventflowEdAppMode.h"
#include "EventflowAsset.h"
#include "Graph/EventflowEdGraph.h"
#include "Graph/EventflowEdGraphNode.h"
#include "Graph/EventflowEdGraphSchema.h"


void FEventflowEdApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit)
{
	WorkingAsset = Cast<UEventflowAsset>(ObjectToEdit);
	WorkingGraph = Cast<UEventflowEdGraph>(FBlueprintEditorUtils::CreateNewGraph(WorkingAsset, NAME_None, GetGraphClass(), GetGraphSchemaClass()));
	
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(ObjectToEdit);

	InitAssetEditor(Mode, InitToolkitHost, TEXT("GraphEditor"), FTabManager::FLayout::NullLayout, true, true, ObjectsToEdit);
	AddApplicationMode(TEXT("RGraphEditorAppMode"), MakeShareable(new FEventflowEdAppMode(SharedThis(this))));
	SetCurrentMode(TEXT("RGraphEditorAppMode"));

	WorkingGraph->InitializeGraph(WorkingAsset);
}

UEventflowAsset* FEventflowEdApp::GetWorkingAsset() const
{
	return WorkingAsset;
}

UEventflowEdGraph* FEventflowEdApp::GetWorkingGraph() const
{
	return WorkingGraph;
}


void FEventflowEdApp::RegisterGraphEvent(SGraphEditor::FGraphEditorEvents& GraphEvents)
{
	GraphEvents.OnSelectionChanged.BindSP(this, &FEventflowEdApp::OnGraphSelectionChanged);
}

void FEventflowEdApp::RegisterGraphEditor(TSharedPtr<SGraphEditor> GraphEditor)
{
	GraphEditorSlate = GraphEditor;
}

void FEventflowEdApp::RegisterGraphProperty(TSharedPtr<IDetailsView> DetailsView)
{
	GraphPropertySlate = DetailsView;
	GraphPropertySlate->OnFinishedChangingProperties().AddRaw(this, &FEventflowEdApp::OnGraphPropertyChanged);
}

void FEventflowEdApp::RegisterNodeProperty(TSharedPtr<IDetailsView> DetailsView)
{
	NodePropertySlate = DetailsView;
	NodePropertySlate->OnFinishedChangingProperties().AddRaw(this, &FEventflowEdApp::OnNodePropertyChanged);
}


FName FEventflowEdApp::GetToolkitFName() const
{
	return FName(TEXT("Eventflow Editor"));
}

FText FEventflowEdApp::GetBaseToolkitName() const
{
	return FText::FromString(TEXT("Eventflow Editor"));
}

FString FEventflowEdApp::GetWorldCentricTabPrefix() const
{
	return FString(TEXT("Eventflow "));
}

FLinearColor FEventflowEdApp::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Red;
}

void FEventflowEdApp::OnClose()
{
	if (GraphPropertySlate)
	{
		GraphPropertySlate->OnFinishedChangingProperties().Clear();
	}
	if (NodePropertySlate)
	{
		NodePropertySlate->OnFinishedChangingProperties().Clear();
	}

	WorkingAsset = nullptr;
	WorkingGraph = nullptr;

	GraphEditorSlate.Reset();
	GraphPropertySlate.Reset();
	NodePropertySlate.Reset();

	GEngine->ForceGarbageCollection(true);

	FWorkflowCentricApplication::OnClose();
}


TSubclassOf<UEventflowEdGraph> FEventflowEdApp::GetGraphClass() const
{
	return UEventflowEdGraph::StaticClass();
}

TSubclassOf<UEventflowEdGraphSchema> FEventflowEdApp::GetGraphSchemaClass() const
{
	return UEventflowEdGraphSchema::StaticClass();
}


void FEventflowEdApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectedNodes)
{
	TArray<TWeakObjectPtr<UObject>> ObjectsToEdit;
	for (UObject* Node : SelectedNodes)
	{
		UEventflowEdGraphNode* GraphNode = Cast<UEventflowEdGraphNode>(Node);
		if (IsValid(GraphNode))
		{
			ObjectsToEdit.Add(GraphNode);
			break;
		}
	}
	NodePropertySlate->SetObjects(ObjectsToEdit);
}

void FEventflowEdApp::OnGraphPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{

}

void FEventflowEdApp::OnNodePropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = PropertyChangedEvent.GetPropertyName();
	if (GraphEditorSlate)
	{
		UEventflowEdGraphNode* SelectedNode = GetFirstSelectedNode(GraphEditorSlate->GetSelectedNodes());
		if (SelectedNode)
		{
			SelectedNode->SyncRuntimePins();
		}
		GraphEditorSlate->NotifyGraphChanged();
	}
}


UEventflowEdGraphNode* FEventflowEdApp::GetFirstSelectedNode(const FGraphPanelSelectionSet& SelectedNodes) const
{
	for (UObject* Node : SelectedNodes)
	{
		UEventflowEdGraphNode* GraphNode = Cast<UEventflowEdGraphNode>(Node);
		if (GraphNode)
		{
			return GraphNode;
		}
	}

	return nullptr;
}

TArray<FName> FEventflowEdApp::GetTriggerGraphProperties() const
{
	return {
		FName(TEXT("GraphBlueprint"))
	};
}

TArray<FName> FEventflowEdApp::GetTriggerNodeProperties() const
{
	return TArray<FName>();
}


void FEventflowEdApp::SaveAsset_Execute()
{
	WorkingGraph->SerializeGraph(WorkingAsset);

	FAssetEditorToolkit::SaveAsset_Execute();
}

