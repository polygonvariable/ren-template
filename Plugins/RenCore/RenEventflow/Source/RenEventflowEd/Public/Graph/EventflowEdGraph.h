// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "EdGraph/EdGraph.h"

// Generated Headers
#include "EventflowEdGraph.generated.h"

// Module Macros
#define REN_API RENEVENTFLOWED_API

// Forward Declarations
class UEventflowAsset;
class UEventflowEdGraphNode;
struct FEventflowNode;
struct FEventflowPin;
struct FEventflowPinRelation;


/*
 * 
 */
UCLASS(MinimalAPI)
class UEventflowEdGraph : public UEdGraph
{

	GENERATED_BODY()

public:

	void InitializeGraph(UEventflowAsset* GraphAsset);
	void SerializeGraph(UEventflowAsset* GraphAsset);
	
protected:

	void SerializeNode(UEventflowAsset* GraphAsset, UEdGraphNode* Node, FGuid& EntryId, TMap<FGuid, FEventflowNode>& NodeCollection, TMap<FGuid, FEventflowPinRelation>& PinRelation);
	void SerializeTask(UEventflowAsset* GraphAsset, FEventflowNode& NodeDefinition, UEventflowEdGraphNode* EdNode);
	void SerializePin(UEdGraphPin* Pin, FEventflowNode& NodeDefinition, TMap<FGuid, FEventflowPinRelation>& PinRelation);

	void RenderGraph(UEventflowAsset* GraphAsset);
	void RenderNode(const FGuid NodeId, const FEventflowNode& NodeDefinition, TMap<FGuid, UEdGraphPin*>& PinCollection);
	void RenderNodePin(EEdGraphPinDirection Direction, const FEventflowPin& Definition, UEdGraphNode* EdNode, TMap<FGuid, UEdGraphPin*>& Collection);
	void RenderNodePinLink(const TMap<FGuid, FEventflowPinRelation>& PinRelation, TMap<FGuid, UEdGraphPin*>& PinCollection);

};


// Module Macros
#undef REN_API

