// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Graph/EventflowEdGraph.h"

// Project Headers
#include "EventflowAsset.h"
#include "EventflowTask.h"
#include "Graph/EventflowEdGraphNode.h"
#include "Graph/EventflowEdGraphSchema.h"


void UEventflowEdGraph::InitializeGraph(UEventflowAsset* GraphAsset)
{
	RenderGraph(GraphAsset);
}

void UEventflowEdGraph::SerializeGraph(UEventflowAsset* GraphAsset)
{
	if (!IsValid(GraphAsset))
	{
		return;
	}

	TMap<FGuid, FEventflowNodeDefinition>& NodeCollection = GraphAsset->NodeCollection;
	NodeCollection.Empty();

	TMap<FGuid, FEventflowPinRelation>& PinRelation = GraphAsset->PinRelation;
	PinRelation.Empty();

	FGuid& EntryId = GraphAsset->EntryNodeId;
	EntryId.Invalidate();

	for (UEdGraphNode* Node : Nodes)
	{
		SerializeNode(GraphAsset, Node, EntryId, NodeCollection, PinRelation);
	}
}


void UEventflowEdGraph::SerializeNode(UEventflowAsset* GraphAsset, UEdGraphNode* Node, FGuid& EntryId, TMap<FGuid, FEventflowNodeDefinition>& NodeCollection, TMap<FGuid, FEventflowPinRelation>& PinRelation)
{
	UEventflowEdGraphNode* EdNode = Cast<UEventflowEdGraphNode>(Node);
	if (!IsValid(EdNode))
	{
		return;
	}

	FEventflowNodeDefinition NodeDefinition;
	NodeDefinition.Position.X = Node->NodePosX;
	NodeDefinition.Position.Y = Node->NodePosY;
	NodeDefinition.PrimaryTask = DuplicateObject(EdNode->PrimaryTask, GraphAsset);
	//NodeDefinition.SecondaryTasks = EdNode->SecondaryTasks;
	NodeDefinition.RuntimeInputs = EdNode->GetRuntimeInputPins();
	NodeDefinition.RuntimeOutputs = EdNode->GetRuntimeOutputPins();
	NodeDefinition.ClassName = FName(Node->GetClass()->GetName());

	const TArray<UEdGraphPin*>& Pins = Node->Pins;
	for (UEdGraphPin* Pin : Pins)
	{
		SerializePin(Pin, NodeDefinition, PinRelation);
	}

	NodeCollection.Add(Node->NodeGuid, NodeDefinition);

	if (EdNode->GetIsEntryNode())
	{
		EntryId = Node->NodeGuid;
	}
}

void UEventflowEdGraph::SerializePin(UEdGraphPin* Pin, FEventflowNodeDefinition& NodeDefinition, TMap<FGuid, FEventflowPinRelation>& PinRelation)
{
	if (Pin->LinkedTo.Num() == 0)
	{
		return;
	}

	FEventflowPinDefinition PinDefinition;
	PinDefinition.UniqueId = Pin->PinId;
	PinDefinition.Name = Pin->PinName;
	PinDefinition.FriendlyName = Pin->PinFriendlyName;
	PinDefinition.Category = Pin->PinType.PinCategory;
	PinDefinition.bIsConst = Pin->PinType.bIsConst;
	
	if (Pin->Direction == EEdGraphPinDirection::EGPD_Output)
	{
		NodeDefinition.StaticOutputs.Add(PinDefinition);
	}
	else if (Pin->Direction == EEdGraphPinDirection::EGPD_Input)
	{
		NodeDefinition.StaticInputs.Add(PinDefinition);
	}

	if (Pin->Direction == EEdGraphPinDirection::EGPD_Output)
	{
		const TArray<UEdGraphPin*>& LinkedTo = Pin->LinkedTo;
		for (UEdGraphPin* Link : LinkedTo)
		{
			FEventflowPinRelation Relation;
			Relation.LinkedToNode = Link->GetOwningNode()->NodeGuid;
			Relation.LinkedToPin = Link->PinId;

			PinRelation.Add(Pin->PinId, Relation);
		}
	}
}


void UEventflowEdGraph::RenderGraph(UEventflowAsset* GraphAsset)
{
	if (!IsValid(GraphAsset))
	{
		return;
	}

	TMap<FGuid, UEdGraphPin*> PinCollection;

	const TMap<FGuid, FEventflowNodeDefinition>& NodeCollection = GraphAsset->NodeCollection;
	for (const TPair<FGuid, FEventflowNodeDefinition>& Kv : NodeCollection)
	{
		RenderNode(Kv.Key, Kv.Value, PinCollection);
	}

	RenderNodePinLink(GraphAsset->PinRelation, PinCollection);
}

void UEventflowEdGraph::RenderNode(const FGuid NodeId, const FEventflowNodeDefinition& NodeDefinition, TMap<FGuid, UEdGraphPin*>& PinCollection)
{
	const UEventflowEdGraphSchema* EdSchema = Cast<UEventflowEdGraphSchema>(GetSchema());
	UClass* NodeClass = EdSchema->GetRegisteredNodeClass(NodeDefinition.ClassName);
	if (!IsValid(NodeClass))
	{
		return;
	}

	UEventflowEdGraphNode* EdNode = NewObject<UEventflowEdGraphNode>(this, NodeClass);
	if (!IsValid(EdNode))
	{
		return;
	}

	EdNode->AllocateDefaultPins();
	EdNode->NodeGuid = NodeId;
	EdNode->NodePosX = NodeDefinition.Position.X;
	EdNode->NodePosY = NodeDefinition.Position.Y;
	EdNode->PrimaryTask = NodeDefinition.PrimaryTask;
	//EdNode->SecondaryTasks = NodeDefinition.SecondaryTasks;

	const TArray<FEventflowPinDefinition>& OutputPins = NodeDefinition.StaticOutputs;
	for (const FEventflowPinDefinition& Pin : OutputPins)
	{
		RenderNodePin(EEdGraphPinDirection::EGPD_Output, Pin, EdNode, PinCollection);
	}

	const TArray<FEventflowPinDefinition>& InputPins = NodeDefinition.StaticInputs;
	for (const FEventflowPinDefinition& Pin : InputPins)
	{
		RenderNodePin(EEdGraphPinDirection::EGPD_Input, Pin, EdNode, PinCollection);
	}

	AddNode(EdNode, false, false);
}

void UEventflowEdGraph::RenderNodePin(EEdGraphPinDirection Direction, const FEventflowPinDefinition& Definition, UEdGraphNode* EdNode, TMap<FGuid, UEdGraphPin*>& Collection)
{
	FGuid UniqueId = Definition.UniqueId;
	UEdGraphPin* EdPin = nullptr;
	if (Definition.bIsConst)
	{
		EdPin = EdNode->FindPin(Definition.Name);
	}
	else
	{
		EdPin = EdNode->CreatePin(EEdGraphPinDirection::EGPD_Output, Definition.Category, Definition.Name);
		EdPin->PinFriendlyName = Definition.FriendlyName;
		EdPin->PinType.bIsConst = false;
	}

	EdPin->PinId = UniqueId;
	Collection.Add(UniqueId, EdPin);
}

void UEventflowEdGraph::RenderNodePinLink(const TMap<FGuid, FEventflowPinRelation>& PinRelation, TMap<FGuid, UEdGraphPin*>& PinCollection)
{
	const UEdGraphSchema* GraphSchema = GetSchema();
	for (const TPair<FGuid, FEventflowPinRelation>& Kv : PinRelation)
	{
		UEdGraphPin** FoundFrom = PinCollection.Find(Kv.Key);
		UEdGraphPin** FoundTo = PinCollection.Find(Kv.Value.LinkedToPin);
		if (!FoundFrom || !FoundTo)
		{
			continue;
		}

		UEdGraphPin* FromPin = *FoundFrom;
		UEdGraphPin* ToPin = *FoundTo;
		if (!FromPin || !ToPin)
		{
			continue;
		}

		GraphSchema->TryCreateConnection(FromPin, ToPin);
	}
}

