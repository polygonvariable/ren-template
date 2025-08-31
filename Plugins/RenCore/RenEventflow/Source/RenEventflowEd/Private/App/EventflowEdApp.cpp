// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "App/EventflowEdApp.h"

// Engine Headers
#include "Kismet2/BlueprintEditorUtils.h"
#include "GraphEditor.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowData.h"
#include "RenEventflow/Public/EventflowNodeData.h"

#include "RenEventflowEd/Public/App/EventflowEdAppMode.h"
#include "RenEventflowEd/Public/Graph/EventflowEdGraph.h"
#include "RenEventflowEd/Public/Graph/EventflowEdGraphNode.h"
#include "RenEventflowEd/Public/Graph/EventflowEdGraphSchema.h"



void FEventflowEdApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit)
{
	WorkingAsset = Cast<UEventflowAsset>(ObjectToEdit);
	WorkingGraph = Cast<UEventflowEdGraph>(FBlueprintEditorUtils::CreateNewGraph(
		WorkingAsset,
		NAME_None,
		GetGraphClass(),
		GetGraphSchemaClass()
	));
	
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(ObjectToEdit);

	InitAssetEditor(Mode, InitToolkitHost, TEXT("GraphEditor"), FTabManager::FLayout::NullLayout, true, true, ObjectsToEdit);
	AddApplicationMode(TEXT("RGraphEditorAppMode"), MakeShareable(new FEventflowEdAppMode(SharedThis(this))));
	SetCurrentMode(TEXT("RGraphEditorAppMode"));

	WorkingGraph->RegisterNodeClasses();
	WorkingGraph->UpdateGraphData(WorkingAsset);
	WorkingGraph->SyncGraphBlueprint(WorkingAsset->GraphBlueprint);

	EventflowAssetSaved = WorkingAsset->OnEventflowAssetSaved.AddSP(this, &FEventflowEdApp::UpdateWorkingAsset);
}

UEventflowAsset* FEventflowEdApp::GetWorkingAsset() const
{
	return WorkingAsset;
}

UEventflowEdGraph* FEventflowEdApp::GetWorkingGraph() const
{
	return WorkingGraph;
}

void FEventflowEdApp::SetGraphEditorSlate(TSharedPtr<SGraphEditor> InGraphEditor)
{
	GraphEditorSlate = InGraphEditor;
}

void FEventflowEdApp::SetGraphPropertySlate(TSharedPtr<IDetailsView> InDetailsView)
{
	GraphPropertySlate = InDetailsView;
	GraphPropertySlate->OnFinishedChangingProperties().Clear();
	GraphPropertySlate->OnFinishedChangingProperties().AddRaw(this, &FEventflowEdApp::OnGraphPropertyChanged);
}

void FEventflowEdApp::SetNodePropertySlate(TSharedPtr<IDetailsView> InDetailsView)
{
	NodePropertySlate = InDetailsView;
	NodePropertySlate->OnFinishedChangingProperties().Clear();
	NodePropertySlate->OnFinishedChangingProperties().AddRaw(this, &FEventflowEdApp::OnNodePropertyChanged);
}

void FEventflowEdApp::RegisterGraphEditorEvents(SGraphEditor::FGraphEditorEvents& GraphEvents)
{
	GraphEvents.OnSelectionChanged.BindSP(this, &FEventflowEdApp::OnGraphSelectionChanged);
	GraphEvents.OnNodeDoubleClicked.BindSP(this, &FEventflowEdApp::OnGraphNodeDoubleClicked);
}



void FEventflowEdApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectedNodes)
{
	UEventflowEdGraphNode* SelectedNode = GetFirstSelectedNode(SelectedNodes);
	if (SelectedNode)
	{
		NodePropertySlate->SetObject(SelectedNode->GetNodeData());
		return;
	}

	NodePropertySlate->SetObject(nullptr);
}

void FEventflowEdApp::OnGraphNodeDoubleClicked(UEdGraphNode* Node)
{

}

void FEventflowEdApp::OnGraphPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = PropertyChangedEvent.GetPropertyName();
	if (WorkingAsset && WorkingGraph && GetTriggerGraphProperties().Contains(PropertyName))
	{
		WorkingGraph->SyncGraphBlueprint(WorkingAsset->GraphBlueprint);
	}
}

void FEventflowEdApp::OnNodePropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (GraphEditorSlate && GetTriggerNodeProperties().Contains(PropertyChangedEvent.GetPropertyName()))
	{
		UEventflowEdGraphNode* SelectedNode = GetFirstSelectedNode(GraphEditorSlate->GetSelectedNodes());
		if (SelectedNode)
		{
			SelectedNode->SyncPins();
		}

		GraphEditorSlate->NotifyGraphChanged();
	}
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

void FEventflowEdApp::UpdateWorkingAsset()
{
	if (!WorkingAsset || !WorkingGraph)
	{
		return;
	}

	WorkingGraph->UpdateAssetData(WorkingAsset);

	UE_LOG(LogTemp, Warning, TEXT("FEventflowEdApp::UpdateWorkingAsset"));
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

TSubclassOf<UEventflowEdGraph> FEventflowEdApp::GetGraphClass() const
{
	return UEventflowEdGraph::StaticClass();
}

TSubclassOf<UEventflowEdGraphSchema> FEventflowEdApp::GetGraphSchemaClass() const
{
	return UEventflowEdGraphSchema::StaticClass();
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

void FEventflowEdApp::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void FEventflowEdApp::OnClose()
{
	if (WorkingAsset) WorkingAsset->OnEventflowAssetSaved.Remove(EventflowAssetSaved);
	if (WorkingGraph) WorkingGraph->RemoveOnGraphChangedHandler(GraphChangedHandle);

	if (GraphPropertySlate) GraphPropertySlate->OnFinishedChangingProperties().Clear();
	if (NodePropertySlate) NodePropertySlate->OnFinishedChangingProperties().Clear();
	
	WorkingAsset = nullptr;
	WorkingGraph = nullptr;

	GraphEditorSlate.Reset();
	GraphPropertySlate.Reset();
	NodePropertySlate.Reset();

	GEngine->ForceGarbageCollection(true);

	FAssetEditorToolkit::OnClose();
}

