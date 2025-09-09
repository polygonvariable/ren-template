// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"

// Project Headers

// Generated Headers
#include "EventflowEdGraph.generated.h"

// Forward Declarations
class UEventflowAsset;
class UEventflowData;
class UEventflowNode;
class UEventflowPin;
class UEventflowNodeData;
class UEventflowBlueprint;

class UEventflowEdGraphNode;



UCLASS()
class RENEVENTFLOWED_API UEventflowEdGraph : public UEdGraph
{

	GENERATED_BODY()

public:

	void RegisterNodeClasses();

	void UpdateAssetData(UEventflowAsset* GraphAsset);
	void UpdateGraphData(UEventflowAsset* GraphAsset);

	//virtual bool ValidateGraphData();

	void SyncGraphBlueprint(TSubclassOf<UEventflowBlueprint> InGraphBlueprint);

protected:

	bool AddNodePin(EEdGraphPinDirection Direction, UEventflowPin* AssetPin, UEventflowEdGraphNode* UINode, TMap<FGuid, UEdGraphPin*>& PinMap);

	virtual TSubclassOf<UEventflowEdGraphNode> GetRegisteredNodeClass(FName NodeType) const;
	virtual TSubclassOf<UEventflowNode> GetAssetNodeClass() const;
	virtual TSubclassOf<UEventflowPin> GetAssetPinClass() const;

public:

	virtual void AddNode(UEdGraphNode* NodeToAdd, bool bUserAction = false, bool bSelectNewNode = true) override;

private:

	TSubclassOf<UEventflowBlueprint> GraphBlueprint;
	TMap<FName, TSubclassOf<UEventflowEdGraphNode>> NodeTypes;

};

