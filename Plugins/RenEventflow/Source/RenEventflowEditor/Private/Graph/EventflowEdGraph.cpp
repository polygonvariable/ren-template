// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdGraph.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowAsset.h"

#include "RenEventflowEditor/Public/App/EventflowEdAppMode.h"
#include "RenEventflowEditor/Public/Graph/EventflowEdGraph.h"
#include "RenEventflowEditor/Public/Graph/EventflowEdGraphNode.h"
#include "RenEventflowEditor/Public/Graph/EventflowEdGraphPin.h"
#include "RenEventflowEditor/Public/Graph/EventflowEdGraphSchema.h"



void UEventflowEdGraph::RegisterNodeType(FName NodeType, TSubclassOf<UEventflowEdGraphNode> NodeClass)
{
	if (!NodeType.IsValid() || !NodeClass) return;

	NodeTypes.Add(NodeType, NodeClass);
}

void UEventflowEdGraph::UnregisterNodeType(FName NodeType)
{
	NodeTypes.Remove(NodeType);
}

TSubclassOf<UEventflowEdGraphNode> UEventflowEdGraph::GetNodeClassOfType(FName NodeType) const
{
	return NodeTypes.FindRef(NodeType);
}


void UEventflowEdGraph::UpdateAssetData(UEventflowData* AssetData)
{
	if (!AssetData) return;

	// TArray of TPair<From, To> Output->Input Pin Connections
	TArray<TPair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEventflowPin*> PinMap;

	for (UEdGraphNode* UINode : Nodes)
	{
		UEventflowEdGraphNode* UINodeCast = Cast<UEventflowEdGraphNode>(UINode);
		if (!UINodeCast) continue;

		UEventflowNode* AssetNode = NewObject<UEventflowNode>(AssetData);
		AssetNode->NodeGuid = UINode->NodeGuid;
		AssetNode->NodeData = UINodeCast->GetAssetNodeData();
		AssetNode->NodeType = UINodeCast->GetNodeType();
		AssetNode->NodePosition = FVector2D(UINode->NodePosX, UINode->NodePosY);

		for (UEdGraphPin* UIPin : UINode->Pins)
		{
			UEventflowPin* AssetPin = NewObject<UEventflowPin>(AssetNode);
			if (!AssetPin) continue;

			AssetPin->PinGuid = UIPin->PinId;
			AssetPin->PinName = UIPin->PinName;
			AssetPin->PinCategory = UIPin->PinType.PinCategory;
			AssetPin->PinFriendlyName = UIPin->PinFriendlyName;
			//AssetPin->PinParentNode = AssetNode;
			AssetPin->bPinIsConst = UIPin->PinType.bIsConst;

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

		FromPin->PinLinkedTo = ToPin;
	}
}

void UEventflowEdGraph::UpdateGraphData(UEventflowData* AssetData)
{
	if (!AssetData) return;

	const TArray<UEventflowNode*>& AssetNodes = AssetData->Nodes;

	// TArray of TPair<From, To> Output->Input Pin Connections
	TArray<TPair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEdGraphPin*> PinMap;

	for (UEventflowNode* AssetNode : AssetNodes)
	{
		if (!AssetNode) continue;

		TSubclassOf<UEventflowEdGraphNode> NodeClass = GetNodeClassOfType(AssetNode->NodeType);
		if (!NodeClass) continue;

		UEventflowEdGraphNode* UINode = NewObject<UEventflowEdGraphNode>(this, NodeClass);
		if (!UINode) continue;

		UINode->CreateDefaultPins();
		UINode->NodeGuid = AssetNode->NodeGuid;
		UINode->NodePosX = AssetNode->NodePosition.X;
		UINode->NodePosY = AssetNode->NodePosition.Y;

		if (AssetNode->NodeData)
		{
			UINode->SetAssetNodeData(DuplicateObject(AssetNode->NodeData, AssetNode));
		}
		else
		{
			UINode->SetAssetNodeData(NewObject<UEventflowNodeData>(AssetNode));
		}

		for (UEventflowPin* AssetPin : AssetNode->InputPins)
		{
			UEdGraphPin* UIPin;

			if (AssetPin->bPinIsConst)
			{
				UIPin = UINode->FindPin(AssetPin->PinName);
			}
			else
			{
				UIPin = UINode->CreatePin(EEdGraphPinDirection::EGPD_Input, AssetPin->PinCategory, AssetPin->PinName);
				UIPin->PinFriendlyName = AssetPin->PinFriendlyName;
				UIPin->PinType.bIsConst = false;
			}
			if (!UIPin) continue;

			UIPin->PinId = AssetPin->PinGuid;

			PinMap.Add(AssetPin->PinGuid, UIPin);
		}

		for (UEventflowPin* AssetPin : AssetNode->OutputPins)
		{
			UEdGraphPin* UIPin;

			if (AssetPin->bPinIsConst)
			{
				UIPin = UINode->FindPin(AssetPin->PinName);
			}
			else
			{
				UIPin = UINode->CreatePin(EEdGraphPinDirection::EGPD_Output, AssetPin->PinCategory, AssetPin->PinName);
				UIPin->PinFriendlyName = AssetPin->PinFriendlyName;
				UIPin->PinType.bIsConst = false;
			}
			if (!UIPin) continue;

			UIPin->PinId = AssetPin->PinGuid;

			if (AssetPin->PinLinkedTo)
			{
				TPair<FGuid, FGuid> Connection;
				Connection.Key = UIPin->PinId;
				Connection.Value = AssetPin->PinLinkedTo->PinGuid;
				Connections.Add(Connection);
			}

			PinMap.Add(AssetPin->PinGuid, UIPin);
		}

		AddNode(UINode, true, true);
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

