// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdGraph.h"

// Engine Headers
#include "EditorDialogLibrary.h"

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"
#include "RenEventflow/Public/EventflowData.h"
#include "RenEventflow/Public/EventflowNode.h"
#include "RenEventflow/Public/EventflowNodeData.h"
#include "RenEventflow/Public/EventflowPin.h"
#include "RenEventflow/Public/EventflowBlueprint.h"

#include "RenEventflowEd/Public/Graph/EventflowEdGraph.h"
#include "RenEventflowEd/Public/Graph/EventflowEdGraphSchema.h"
#include "RenEventflowEd/Public/Graph/EventflowEdGraphNode.h"



void UEventflowEdGraph::RegisterNodeClasses()
{
	const UEventflowEdGraphSchema* CurrentSchema = Cast<UEventflowEdGraphSchema>(GetSchema());
	if (!CurrentSchema) return;

	TArray<UClass*> NodeClasses = CurrentSchema->GetNodeClasses();
	for (UClass* NodeClass : NodeClasses)
	{
		UEventflowEdGraphNode* Node = NodeClass->GetDefaultObject<UEventflowEdGraphNode>();
		if (!Node) continue;

		NodeTypes.Add(Node->GetNodeType(), NodeClass);
	}
}

void UEventflowEdGraph::UpdateAssetData(UEventflowAsset* GraphAsset)
{
	if (!GraphAsset)
	{
		return;
	}

	if (GraphAsset->GraphData)
	{
		GraphAsset->GraphData->MarkAsGarbage();
		GraphAsset->GraphData = nullptr;
	}
	GEngine->ForceGarbageCollection(true);

	UEventflowData* GraphData = NewObject<UEventflowData>(GraphAsset);
	if (!GraphData) return;

	// TArray of TPair<From, To> Output->Input Pin Connections
	TArray<TPair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEventflowPin*> PinMap;

	int Count = Nodes.Num();
	for (int i = 0; i < Count; i++)
	{
		UEventflowEdGraphNode* UINode = Cast<UEventflowEdGraphNode>(Nodes[i]);
		if (!UINode) continue;

		FGuid NodeGuid = (UINode->NodeGuid.IsValid()) ? UINode->NodeGuid : FGuid::NewGuid();

		if (UINode->IsEntryNode())
		{
			GraphData->NodeEntry = NodeGuid;
		}

		UEventflowNode* AssetNode = NewObject<UEventflowNode>(GraphData, GetAssetNodeClass());
		AssetNode->NodeId = NodeGuid;
		AssetNode->NodeData = DuplicateObject(UINode->GetNodeData(), AssetNode);
		AssetNode->NodeType = UINode->GetNodeType();
		AssetNode->NodePosition = FVector2D(UINode->NodePosX, UINode->NodePosY);

		for (UEdGraphPin* UIPin : UINode->Pins)
		{
			UEventflowPin* AssetPin = NewObject<UEventflowPin>(AssetNode, GetAssetPinClass());
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
				AssetNode->NodeInputs.Add(AssetPin);
			}
			else
			{
				AssetNode->NodeOutputs.Add(AssetPin);
			}
		}

		GraphData->NodeList.Add(NodeGuid, AssetNode);
	}

	for (TPair<FGuid, FGuid> Connection : Connections)
	{
		UEventflowPin* FromPin = PinMap[Connection.Key];
		UEventflowPin* ToPin = PinMap[Connection.Value];

		FromPin->PinLinkedTo = ToPin;
	}

	GraphAsset->GraphData = GraphData;
}

void UEventflowEdGraph::UpdateGraphData(UEventflowAsset* GraphAsset)
{
	if (!GraphAsset)
	{
		return;
	}

	UEventflowData* GraphData = GraphAsset->GraphData;
	if (!GraphData)
	{
		return;
	}
	// TArray of TPair<From, To> Output->Input Pin Connections
	TArray<TPair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEdGraphPin*> PinMap;

	const TMap<FGuid, TObjectPtr<UEventflowNode>>& NodeList = GraphData->NodeList;

	for (const TTuple<FGuid, TObjectPtr<UEventflowNode>>& Kv : NodeList)
	{
		FGuid NodeGuid = (Kv.Key.IsValid()) ? Kv.Key : FGuid::NewGuid();
		UEventflowNode* AssetNode = Kv.Value.Get();

		if (!AssetNode) continue;

		TSubclassOf<UEventflowEdGraphNode> NodeClass = GetRegisteredNodeClass(AssetNode->NodeType);
		if (!NodeClass) continue;

		UEventflowEdGraphNode* UINode = NewObject<UEventflowEdGraphNode>(this, NodeClass);
		if (!UINode) continue;

		TSubclassOf<UEventflowNodeData> NodeDataClass = UINode->GetNodeDataClass();
		if (!NodeDataClass) continue;

		UINode->AllocateDefaultPins();
		UINode->NodeGuid = NodeGuid;
		UINode->NodePosX = AssetNode->NodePosition.X;
		UINode->NodePosY = AssetNode->NodePosition.Y;

		if (AssetNode->NodeData)
		{
			UINode->SetNodeData(DuplicateObject(AssetNode->NodeData, UINode));
		}
		else
		{
			UINode->SetNodeData(NewObject<UEventflowNodeData>(UINode, NodeDataClass));
		}

		for (UEventflowPin* AssetPin : AssetNode->NodeInputs)
		{
			AddNodePin(EEdGraphPinDirection::EGPD_Input, AssetPin, UINode, PinMap);
		}
		
		for (UEventflowPin* AssetPin : AssetNode->NodeOutputs)
		{
			if (AddNodePin(EEdGraphPinDirection::EGPD_Output, AssetPin, UINode, PinMap) && AssetPin->PinLinkedTo)
			{
				TPair<FGuid, FGuid> Connection;
				Connection.Key = AssetPin->PinGuid;
				Connection.Value = AssetPin->PinLinkedTo->PinGuid;
				Connections.Add(Connection);
			}
		}

		AddNode(UINode, false, false);
	}

	for (TPair<FGuid, FGuid> Connection : Connections)
	{
		UEdGraphPin* FromPin = PinMap[Connection.Key];
		UEdGraphPin* ToPin = PinMap[Connection.Value];

		if (!FromPin || !ToPin) continue;

		GetSchema()->TryCreateConnection(FromPin, ToPin);
	}
}

/*
bool UEventflowEdGraph::ValidateGraphData()
{
	int EntryNodes = 0;
	const TArray<TObjectPtr<UEdGraphNode>>& CachedNodes = Nodes;

	for (TObjectPtr<UEdGraphNode> CachedNode : CachedNodes)
	{
		UEventflowEdGraphNode* Node = Cast<UEventflowEdGraphNode>(CachedNode);
		if (!Node)
		{
			UEditorDialogLibrary::ShowMessage(FText::FromString(TEXT("Error")), FText::FromString(TEXT("Node is not of type UEventflowEdGraphNode")), EAppMsgType::Ok, EAppReturnType::Ok, EAppMsgCategory::Error);
			return false;
		}

		if (Node->IsEntryNode())
		{
			EntryNodes++;
		}

		if (EntryNodes > 1)
		{
			UEditorDialogLibrary::ShowMessage(FText::FromString(TEXT("Error")), FText::FromString(TEXT("Multiple Entry points found")), EAppMsgType::Ok, EAppReturnType::Ok, EAppMsgCategory::Error);
			return false;
		}
	}

	if (EntryNodes == 0)
	{
		UEditorDialogLibrary::ShowMessage(FText::FromString(TEXT("Error")), FText::FromString(TEXT("No Entry point found")), EAppMsgType::Ok, EAppReturnType::Ok, EAppMsgCategory::Error);
		return false;
	}

	return true;
}*/

void UEventflowEdGraph::SyncGraphBlueprint(TSubclassOf<UEventflowBlueprint> InGraphBlueprint)
{
	GraphBlueprint = InGraphBlueprint;

	for (UEdGraphNode* Node : Nodes)
	{
		UEventflowEdGraphNode* UINode = Cast<UEventflowEdGraphNode>(Node);
		if (UINode)
		{
			UINode->SyncBlueprintGraph(InGraphBlueprint);
		}
	}
}



bool UEventflowEdGraph::AddNodePin(EEdGraphPinDirection Direction, UEventflowPin* AssetPin, UEventflowEdGraphNode* UINode, TMap<FGuid, UEdGraphPin*>& PinMap)
{
	if (!AssetPin) return false;

	UEdGraphPin* UIPin;

	if (AssetPin->bPinIsConst)
	{
		UIPin = UINode->FindPin(AssetPin->PinName);
	}
	else
	{
		UIPin = UINode->CreatePin(Direction, AssetPin->PinCategory, AssetPin->PinName);
		UIPin->PinFriendlyName = AssetPin->PinFriendlyName;
		UIPin->PinType.bIsConst = false;
	}
	if (!UIPin) return false;

	UIPin->PinId = AssetPin->PinGuid;

	PinMap.Add(AssetPin->PinGuid, UIPin);

	return true;
}

TSubclassOf<UEventflowEdGraphNode> UEventflowEdGraph::GetRegisteredNodeClass(FName NodeType) const
{
	return NodeTypes.FindRef(NodeType);
}

TSubclassOf<UEventflowNode> UEventflowEdGraph::GetAssetNodeClass() const
{
	return UEventflowNode::StaticClass();
}

TSubclassOf<UEventflowPin> UEventflowEdGraph::GetAssetPinClass() const
{
	return UEventflowPin::StaticClass();
}

void UEventflowEdGraph::AddNode(UEdGraphNode* NodeToAdd, bool bUserAction, bool bSelectNewNode)
{
	UEventflowEdGraphNode* Node = Cast<UEventflowEdGraphNode>(NodeToAdd);
	if (Node)
	{
		Node->SyncBlueprintGraph(GraphBlueprint);
	}
	Super::AddNode(NodeToAdd, bUserAction, bSelectNewNode);
}

