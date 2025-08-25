// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"

// Project Headers

// Generated Headers
#include "EventflowEdGraph.generated.h"

// Forward Declarations
class UEventflowData;

class UEventflowEdGraphNode;



UCLASS()
class UEventflowEdGraph : public UEdGraph
{

	GENERATED_BODY()

public:

	void RegisterNodeType(FName NodeType, TSubclassOf<UEventflowEdGraphNode> NodeClass);
	void UnregisterNodeType(FName NodeType);

	TSubclassOf<UEventflowEdGraphNode> GetNodeClassOfType(FName NodeType) const;

	void UpdateAssetData(UEventflowData* AssetData);
	void UpdateGraphData(UEventflowData* AssetData);

private:

	TMap<FName, TSubclassOf<UEventflowEdGraphNode>> NodeTypes;

};

