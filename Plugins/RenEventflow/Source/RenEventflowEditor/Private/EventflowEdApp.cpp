// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowEdApp.h"

// Engine Headers
#include "Kismet2/BlueprintEditorUtils.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEditor/Public/EventflowEdAppMode.h"
#include "RenEventflowEditor/Public/EventflowEdGraphSchema.h"
#include "RenEventflowEditor/Public/EventflowEdGraphNode.h"
#include "RenEventflowEditor/Public/EventflowEdGraphPin.h"



void EventflowEdApp::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void EventflowEdApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit)
{
	WorkingAsset = Cast<UEventflowAsset>(ObjectToEdit);
	WorkingGraph = FBlueprintEditorUtils::CreateNewGraph(
		WorkingAsset,
		NAME_None,
		UEdGraph::StaticClass(),
		UEventflowEdGraphSchema::StaticClass()
	);

	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(ObjectToEdit);

	InitAssetEditor(Mode, InitToolkitHost, TEXT("GraphEditor"), FTabManager::FLayout::NullLayout, true, true, ObjectsToEdit);

	AddApplicationMode(TEXT("RGraphEditorAppMode"), MakeShareable(new EventflowEdAppMode(SharedThis(this))));

	SetCurrentMode(TEXT("RGraphEditorAppMode"));

	UpdateEditorGraphFromWorkingAsset();

	GraphChangedHandle = WorkingGraph->AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateSP(this, &EventflowEdApp::OnGraphChanged));
}

void EventflowEdApp::OnClose()
{
	UpdateWorkingAssetFromGraph();
	WorkingGraph->RemoveOnGraphChangedHandler(GraphChangedHandle);

	FAssetEditorToolkit::OnClose();
}

void EventflowEdApp::OnGraphChanged(const FEdGraphEditAction& InAction)
{
	UpdateWorkingAssetFromGraph();
}



void EventflowEdApp::OnNodeDetailsChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (WorkingGraphEditor)
	{
		UEventflowEdGraphNode* SelectedNode = GetSelectedNode(WorkingGraphEditor->GetSelectedNodes());
		if (SelectedNode)
		{
			SelectedNode->SyncOutputPins();
		}

		WorkingGraphEditor->NotifyGraphChanged();
	}
}

void EventflowEdApp::SetWorkingGraphEditor(TSharedPtr<SGraphEditor> InGraphEditor)
{
	WorkingGraphEditor = InGraphEditor;
}

void EventflowEdApp::SetSelectedNodeDetail(TSharedPtr<IDetailsView> InDetailsView)
{
	SelectedNodeDetail = InDetailsView;
	SelectedNodeDetail->OnFinishedChangingProperties().AddRaw(this, &EventflowEdApp::OnNodeDetailsChanged);



}

void EventflowEdApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& SelectedNodes)
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

UEventflowEdGraphNode* EventflowEdApp::GetSelectedNode(const FGraphPanelSelectionSet& SelectedNodes) const
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




FName EventflowEdApp::GetToolkitFName() const
{
	return FName(TEXT("GraphEditorApp"));
}

FText EventflowEdApp::GetBaseToolkitName() const
{
	return FText::FromString(TEXT("GraphEditorApp"));
}

FString EventflowEdApp::GetWorldCentricTabPrefix() const
{
	return FString(TEXT("GraphEditorApp"));
}

FLinearColor EventflowEdApp::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Red;
}





void EventflowEdApp::UpdateWorkingAssetFromGraph()
{
	if (!WorkingAsset || !WorkingGraph)
	{
		return;
	}

	UEventflowData* AssetData = NewObject<UEventflowData>(WorkingAsset);
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
		AssetNode->Position = FVector2D(UINode->NodePosX, UINode->NodePosY);
		AssetNode->NodeData = UINodeCast->GetAssetNodeData();
		AssetNode->NType = UINodeCast->NType;

		for (UEdGraphPin* UIPin : UINode->Pins)
		{
			UEventflowPin* AssetPin = NewObject<UEventflowPin>(AssetNode);
			AssetPin->PinName = UIPin->PinName;
			AssetPin->PinGuid = UIPin->PinId;

			if (UIPin->HasAnyConnections() && UIPin->Direction == EEdGraphPinDirection::EGPD_Output)
			{
				// TPair<From, To>
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

		FromPin->NextPin = ToPin;
	}
}

void EventflowEdApp::UpdateEditorGraphFromWorkingAsset()
{
	if (!WorkingAsset->GraphData)
	{
		return;
	}

	TArray<UEventflowNode*> AssetNodes = WorkingAsset->GraphData->Nodes;

	// TArray of TPair<From, To> Output->Input Pin Connections
	TArray<TPair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEdGraphPin*> PinMap;

	for (UEventflowNode* AssetNode : AssetNodes)
	{
		if (!AssetNode) continue;
		UEventflowEdGraphNode* UINode;

		if (AssetNode->NType == TEXT("BEGIN"))
		{
			UINode = NewObject<UEventflowEdGraphBeginNode>(WorkingGraph);
		}
		else if (AssetNode->NType == TEXT("END"))
		{
			UINode = NewObject<UEventflowEdGraphEndNode>(WorkingGraph);
		}
		else
		{
			continue;
			// UINode = NewObject<UEventflowEdGraphNode>(WorkingGraph);
		}


		UINode->CreateDefaultPins();
		UINode->NodeGuid = AssetNode->NodeGuid;
		UINode->NodePosX = AssetNode->Position.X;
		UINode->NodePosY = AssetNode->Position.Y;

		if (AssetNode->NodeData)
		{
			UINode->SetAssetNodeData(DuplicateObject(AssetNode->NodeData, AssetNode));
		}
		else
		{
			UINode->SetAssetNodeData(NewObject<UEventflowNodeData>(AssetNode));
		}

		/*if (AssetNode->InputPin)
		{
			UEventflowPin* AssetPin = AssetNode->InputPin;
			UEdGraphPin* UIPin = UINode->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, AssetPin->PinName);
			UIPin->PinId = AssetPin->PinId;

			if (AssetPin->NextPin)
			{
				TPair<FGuid, FGuid> Connection;
				Connection.Key = UIPin->PinId;
				Connection.Value = AssetPin->NextPin->PinId;
				Connections.Add(Connection);
			}

			PinMap.Add(AssetPin->PinId, UIPin);
		}*/

		for (UEventflowPin* AssetPin : AssetNode->InputPins)
		{
			/*UEdGraphPin* UIPin;

			if (!AssetPin->bIsConst)
			{
				UIPin = UINode->CreatePinHelper(EEdGraphPinDirection::EGPD_Input, AssetPin->PinName, TEXT("CUSTOM_PIN"), true);
				UIPin->PinId = AssetPin->PinGuid;
			}
			else
			{
				UIPin = UINode->FindPinById(AssetPin->PinGuid);
			}*/

			UEdGraphPin* UIPin = UINode->CreatePinHelper(EEdGraphPinDirection::EGPD_Input, AssetPin->PinName, TEXT("CUSTOM_PIN"), true);
			UIPin->PinId = AssetPin->PinGuid;

			PinMap.Add(AssetPin->PinGuid, UIPin);
		}

		for (UEventflowPin* AssetPin : AssetNode->OutputPins)
		{
			UEdGraphPin* UIPin = UINode->CreatePinHelper(EEdGraphPinDirection::EGPD_Output, AssetPin->PinName, TEXT("CUSTOM_PIN"), true);
			UIPin->PinId = AssetPin->PinGuid;

			if (AssetPin->NextPin)
			{
				TPair<FGuid, FGuid> Connection;
				Connection.Key = UIPin->PinId;
				Connection.Value = AssetPin->NextPin->PinGuid;
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

		FromPin->LinkedTo.Add(ToPin);
		ToPin->LinkedTo.Add(FromPin);
	}

}




