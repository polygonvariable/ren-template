// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "App/EventflowEdApp.h"

// Engine Headers
#include "Kismet2/BlueprintEditorUtils.h"
#include "UObject/UObjectBaseUtility.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEditor/Public/App/EventflowEdAppMode.h"
#include "RenEventflowEditor/Public/Graph/EventflowEdGraph.h"
#include "RenEventflowEditor/Public/Graph/EventflowEdGraphNode.h"
#include "RenEventflowEditor/Public/Graph/EventflowEdGraphPin.h"
#include "RenEventflowEditor/Public/Graph/EventflowEdGraphSchema.h"



void FEventflowEdApp::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

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

	RegisterGraphNodes();
	
	InitAssetEditor(Mode, InitToolkitHost, TEXT("GraphEditor"), FTabManager::FLayout::NullLayout, true, true, ObjectsToEdit);
	AddApplicationMode(TEXT("RGraphEditorAppMode"), MakeShareable(new FEventflowEdAppMode(SharedThis(this))));
	SetCurrentMode(TEXT("RGraphEditorAppMode"));
	UpdateEditorGraphFromWorkingAsset();

	EventflowAssetSaved = WorkingAsset->OnEventflowAssetSaved.AddSP(this, &FEventflowEdApp::UpdateWorkingAssetFromGraph);
	GraphChangedHandle = WorkingGraph->AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateSP(this, &FEventflowEdApp::OnGraphChanged));
}

void FEventflowEdApp::OnClose()
{
	if (WorkingAsset) WorkingAsset->OnEventflowAssetSaved.Remove(EventflowAssetSaved);
	if (WorkingGraph) WorkingGraph->RemoveOnGraphChangedHandler(GraphChangedHandle);
	if (SelectedNodeDetail) SelectedNodeDetail->OnFinishedChangingProperties().Clear();

	WorkingAsset = nullptr;

	GEngine->ForceGarbageCollection(true);

	FAssetEditorToolkit::OnClose();
}

void FEventflowEdApp::OnGraphChanged(const FEdGraphEditAction& InAction)
{
	UpdateWorkingAssetFromGraph();
}



void FEventflowEdApp::OnNodeDetailsChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (WorkingGraphEditor)
	{
		UEventflowEdGraphNode* SelectedNode = GetSelectedNode(WorkingGraphEditor->GetSelectedNodes());
		if (SelectedNode)
		{
			SelectedNode->SyncPins();
		}

		WorkingGraphEditor->NotifyGraphChanged();
	}
}

void FEventflowEdApp::SetWorkingGraphEditor(TSharedPtr<SGraphEditor> InGraphEditor)
{
	WorkingGraphEditor = InGraphEditor;
}

void FEventflowEdApp::SetSelectedNodeDetail(TSharedPtr<IDetailsView> InDetailsView)
{
	SelectedNodeDetail = InDetailsView;
	SelectedNodeDetail->OnFinishedChangingProperties().AddRaw(this, &FEventflowEdApp::OnNodeDetailsChanged);
}

void FEventflowEdApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectedNodes)
{
	UEventflowEdGraphNode* SelectedNode = GetSelectedNode(SelectedNodes);
	if (SelectedNode)
	{
		SelectedNodeDetail->SetObject(SelectedNode->GetAssetNodeData());
	}
	else
	{
		SelectedNodeDetail->SetObject(nullptr);
	}
}

UEventflowEdGraphNode* FEventflowEdApp::GetSelectedNode(const FGraphPanelSelectionSet& SelectedNodes) const
{
	for (UObject* Node : SelectedNodes)
	{
		UEventflowEdGraphNode* NodeCast = Cast<UEventflowEdGraphNode>(Node);
		if (NodeCast)
		{
			return NodeCast;
		}
	}

	return nullptr;
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





void FEventflowEdApp::RegisterGraphNodes()
{
	WorkingGraph->RegisterNodeType(UEventflowEdGraphBeginNode::StaticNodeType(), UEventflowEdGraphBeginNode::StaticClass());
	WorkingGraph->RegisterNodeType(UEventflowEdGraphEndNode::StaticNodeType(), UEventflowEdGraphEndNode::StaticClass());
}

TSubclassOf<UEventflowEdGraph> FEventflowEdApp::GetGraphClass() const
{
	return UEventflowEdGraph::StaticClass();
}

TSubclassOf<UEventflowEdGraphSchema> FEventflowEdApp::GetGraphSchemaClass() const
{
	return UEventflowEdGraphSchema::StaticClass();
}

void FEventflowEdApp::UpdateWorkingAssetFromGraph()
{
	if (!WorkingAsset || !WorkingGraph)
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
	//WorkingGraph->UpdateAssetData(AssetData);

	WorkingAsset->GraphData = AssetData;

	// TArray of TPair<From, To> Output->Input Pin Connections
	TArray<TPair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEventflowPin*> PinMap;

	for (UEdGraphNode* UINode : WorkingGraph->Nodes)
	{
		UEventflowEdGraphNode* UINodeCast = Cast<UEventflowEdGraphNode>(UINode);
		if (!UINodeCast) continue;
		
		UEventflowNode* AssetNode = NewObject<UEventflowNode>(AssetData);
		AssetNode->NodeGuid = UINode->NodeGuid;
		AssetNode->NodeData = DuplicateObject(UINodeCast->GetAssetNodeData(), AssetNode);
		AssetNode->NodeType = UINodeCast->GetNodeType();
		AssetNode->NodePosition = FVector2D(UINode->NodePosX, UINode->NodePosY);

		for (UEdGraphPin* UIPin : UINode->Pins)
		{
			UEventflowPin* AssetPin = NewObject<UEventflowPin>(AssetNode);
			if (!AssetPin) continue;

			AssetPin->PinGuid = UIPin->PinId;
			AssetPin->PinName = UIPin->PinName;
			AssetPin->PinFriendlyName = UIPin->PinFriendlyName;
			AssetPin->PinCategory = UIPin->PinType.PinCategory;
			AssetPin->bPinIsConst = UIPin->PinType.bIsConst;

			if (UIPin->HasAnyConnections() && UIPin->Direction == EEdGraphPinDirection::EGPD_Output)
			{
				TPair<FGuid, FGuid> Connection;
				Connection.Key = UIPin->PinId;
				Connection.Value = UIPin->LinkedTo[0]->PinId;
				Connections.Add(Connection);
			}

			PinMap.Add(UIPin->PinId, AssetPin);
			if (UIPin->Direction == EEdGraphPinDirection::EGPD_Input)
			{
				AssetNode->InputPins.Add(AssetPin);
			}
			else
			{
				AssetNode->OutputPins.Add(AssetPin);
			}
		}
		
		AssetData->Nodes.Add(AssetNode);
	}

	for (TPair<FGuid, FGuid> Connection : Connections)
	{
		UEventflowPin* FromPin = PinMap[Connection.Key];
		UEventflowPin* ToPin = PinMap[Connection.Value];

		FromPin->PinLinkedTo = ToPin;
	}

	UE_LOG(LogTemp, Warning, TEXT("FEventflowEdApp::UpdateWorkingAssetFromGraph"));
}

void FEventflowEdApp::UpdateEditorGraphFromWorkingAsset()
{
	if (!WorkingAsset->GraphData)
	{
		return;
	}
	
	//WorkingGraph->UpdateGraphData(WorkingAsset->GraphData);
	
	TArray<UEventflowNode*> AssetNodes = WorkingAsset->GraphData->Nodes;
	
	// TArray of TPair<From, To> Output->Input Pin Connections
	TArray<TPair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEdGraphPin*> PinMap;

	for (UEventflowNode* AssetNode : AssetNodes)
	{
		if (!AssetNode) continue;

		TSubclassOf<UEventflowEdGraphNode> NodeClass = WorkingGraph->GetNodeClassOfType(AssetNode->NodeType);
		if (!NodeClass) continue;

		UEventflowEdGraphNode* UINode = NewObject<UEventflowEdGraphNode>(WorkingGraph, NodeClass);
		if (!UINode) continue;

		UINode->CreateDefaultPins();
		UINode->NodeGuid = AssetNode->NodeGuid;
		UINode->NodePosX = AssetNode->NodePosition.X;
		UINode->NodePosY = AssetNode->NodePosition.Y;

		if (AssetNode->NodeData)
		{
			UINode->SetAssetNodeData(DuplicateObject(AssetNode->NodeData, UINode));
		}
		else
		{
			UINode->SetAssetNodeData(NewObject<UEventflowNodeData>(UINode));
		}

		for (UEventflowPin* AssetPin : AssetNode->InputPins)
		{
			if (!AssetPin) continue;

			UEdGraphPin* UIPin;

			if (AssetPin->bPinIsConst)
			{
				UIPin = UINode->FindPin(AssetPin->PinName);
				UIPin->PinId = AssetPin->PinGuid;
			}
			else
			{
				UIPin = UINode->CreatePin(EEdGraphPinDirection::EGPD_Input, TEXT("Runtime"), AssetPin->PinName);
				UIPin->PinId = AssetPin->PinGuid;
				UIPin->PinFriendlyName = AssetPin->PinFriendlyName;
				UIPin->PinType.bIsConst = false;
			}
			if (!UIPin) continue;


			PinMap.Add(AssetPin->PinGuid, UIPin);
		}
		
		for (UEventflowPin* AssetPin : AssetNode->OutputPins)
		{
			if (!AssetPin) continue;

			UEdGraphPin* UIPin;

			if (AssetPin->bPinIsConst)
			{
				UIPin = UINode->FindPin(AssetPin->PinName);
				UIPin->PinId = AssetPin->PinGuid;
			}
			else
			{
				UIPin = UINode->CreatePin(EEdGraphPinDirection::EGPD_Output, TEXT("Runtime"), AssetPin->PinName);
				UIPin->PinId = AssetPin->PinGuid;
				UIPin->PinFriendlyName = AssetPin->PinFriendlyName;
				UIPin->PinType.bIsConst = false;
			}
			if (!UIPin) continue;


			if (AssetPin->PinLinkedTo)
			{
				TPair<FGuid, FGuid> Connection;
				Connection.Key = UIPin->PinId;
				Connection.Value = AssetPin->PinLinkedTo->PinGuid;
				Connections.Add(Connection);
			}

			PinMap.Add(AssetPin->PinGuid, UIPin);
		}

		WorkingGraph->AddNode(UINode, true, true);
	}

	for (TPair<FGuid, FGuid> Connection : Connections)
	{
		UEdGraphPin* FromPin = PinMap[Connection.Key];
		UEdGraphPin* ToPin = PinMap[Connection.Value];

		if (!FromPin || !ToPin) continue;

		FromPin->LinkedTo.Add(ToPin);
		ToPin->LinkedTo.Add(FromPin);
	}

}




