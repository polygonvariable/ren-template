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
class UEventflowNode;
class UEventflowPin;
class UEventflowNodeData;

class UEventflowEdGraphNode;



UCLASS()
class RENEVENTFLOWEDITOR_API UEventflowEdGraph : public UEdGraph
{

	GENERATED_BODY()

public:

	void RegisterNodeClasses();

	void UpdateAssetData(UEventflowData* AssetData);
	void UpdateGraphData(UEventflowData* AssetData);

	virtual bool ValidateGraphData();

protected:

	bool AddNodePin(EEdGraphPinDirection Direction, UEventflowPin* AssetPin, UEventflowEdGraphNode* UINode, TMap<FGuid, UEdGraphPin*>& PinMap);

	virtual TSubclassOf<UEventflowEdGraphNode> GetRegisteredNodeClass(FName NodeType) const;
	virtual TSubclassOf<UEventflowNode> GetAssetNodeClass() const;
	virtual TSubclassOf<UEventflowPin> GetAssetPinClass() const;

private:

	TMap<FName, TSubclassOf<UEventflowEdGraphNode>> NodeTypes;

};

