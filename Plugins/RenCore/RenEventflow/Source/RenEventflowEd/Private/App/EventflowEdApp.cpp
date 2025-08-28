// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "App/EventflowEdApp.h"

// Engine Headers
#include "Kismet2/BlueprintEditorUtils.h"

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
	UpdateWorkingGraph();

	EventflowAssetSaved = WorkingAsset->OnEventflowAssetSaved.AddSP(this, &FEventflowEdApp::UpdateWorkingAsset);
	GraphChangedHandle = WorkingGraph->AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateSP(this, &FEventflowEdApp::OnGraphChanged));
}

UEventflowAsset* FEventflowEdApp::GetWorkingAsset() const
{
	return WorkingAsset;
}

UEventflowEdGraph* FEventflowEdApp::GetWorkingGraph() const
{
	return WorkingGraph;
}

void FEventflowEdApp::SetWorkingGraphEditor(TSharedPtr<SGraphEditor> InGraphEditor)
{
	WorkingGraphEditor = InGraphEditor;
}

void FEventflowEdApp::SetSelectedNodeDetail(TSharedPtr<IDetailsView> InDetailsView)
{
	SelectedNodeDetail = InDetailsView;
	SelectedNodeDetail->OnFinishedChangingProperties().Clear();
	SelectedNodeDetail->OnFinishedChangingProperties().AddRaw(this, &FEventflowEdApp::OnNodeDetailsChanged);
}

void FEventflowEdApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectedNodes)
{
	UEventflowEdGraphNode* SelectedNode = GetFirstSelectedNode(SelectedNodes);
	if (SelectedNode)
	{
		SelectedNodeDetail->SetObject(SelectedNode->GetNodeData());
		return;
	}

	SelectedNodeDetail->SetObject(nullptr);
}

void FEventflowEdApp::OnGraphNodeDoubleClicked(UEdGraphNode* Node)
{

}



void FEventflowEdApp::OnGraphChanged(const FEdGraphEditAction& InAction)
{
	UpdateWorkingAsset();
}

void FEventflowEdApp::OnNodeDetailsChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (WorkingGraphEditor)
	{
		UEventflowEdGraphNode* SelectedNode = GetFirstSelectedNode(WorkingGraphEditor->GetSelectedNodes());
		if (SelectedNode)
		{
			SelectedNode->SyncPins();
		}
		
		WorkingGraphEditor->NotifyGraphChanged();
	}
}

void FEventflowEdApp::UpdateWorkingAsset()
{
	if (!WorkingAsset || !WorkingGraph)
	{
		return;
	}

	if (!WorkingGraph->ValidateGraphData())
	{
		return;
	}

	if (WorkingAsset->GraphData)
	{
		WorkingAsset->GraphData->MarkAsGarbage();
		WorkingAsset->GraphData = nullptr;
	}
	GEngine->ForceGarbageCollection(true);

	UEventflowData* AssetData = NewObject<UEventflowData>(WorkingAsset);
	WorkingGraph->UpdateAssetData(AssetData);
	WorkingAsset->GraphData = AssetData;

	UE_LOG(LogTemp, Warning, TEXT("FEventflowEdApp::UpdateWorkingAsset"));
}

void FEventflowEdApp::UpdateWorkingGraph()
{
	if (!WorkingAsset || !WorkingGraph)
	{
		return;
	}

	WorkingGraph->UpdateGraphData(WorkingAsset->GraphData);
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
	if (SelectedNodeDetail) SelectedNodeDetail->OnFinishedChangingProperties().Clear();
	
	WorkingAsset = nullptr;
	WorkingGraph = nullptr;

	SelectedNodeDetail.Reset();
	WorkingGraphEditor.Reset();

	GEngine->ForceGarbageCollection(true);

	FAssetEditorToolkit::OnClose();
}

