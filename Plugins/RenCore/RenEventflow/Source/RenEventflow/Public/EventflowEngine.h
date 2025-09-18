// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEventflow/Public/EventflowDelegates.h"

// Generated Headers
#include "EventflowEngine.generated.h"

// Forward Declarations
class UEventflowAsset;
class UEventflowNode;
class UEventflowData;
class UEventflowBlueprint;



/**
 *
 *
 */
UCLASS()
class RENEVENTFLOW_API UEventflowEngine : public UObject
{

	GENERATED_BODY()

public:

	void LoadAsset(const FPrimaryAssetId& AssetId);
	void LoadAsset(UEventflowAsset* Asset);

	void UnloadAsset();

	UEventflowNode* GetNodeById(FGuid NodeId) const;

	bool ReachEntryNode();
	bool ReachNode(UEventflowNode* Node);
	bool ReachNodeById(FGuid NodeId);
	bool ReachNextNode(UEventflowNode* Node, int Index);
	bool ReachImmediateNextNode(UEventflowNode* Node);

	bool ExecuteNode(UEventflowNode* Node);

	FPrimaryAssetId GetOwningAssetId() const;
	UEventflowAsset* GetOwningAsset() const;
	UEventflowBlueprint* GetCurrentBlueprint() const;

	FOnGraphStarted OnGraphStarted;
	FOnGraphEnded OnGraphEnded;

protected:

	UPROPERTY()
	FPrimaryAssetId CurrentAssetId;

	UPROPERTY()
	TObjectPtr<UEventflowBlueprint> CurrentBlueprint;

	UPROPERTY()
	TObjectPtr<UEventflowAsset> CurrentAsset;

	UPROPERTY()
	TObjectPtr<UEventflowData> CachedGraphData;

	void InitializeEngine();

	virtual void ConstructBlueprint(TSubclassOf<UObject> InClass);
	virtual void DestructBlueprint();

	virtual void HandleAssetLoaded();

	virtual void HandleOnNodeReached(UEventflowNode* Node);
	virtual void HandleOnNodeExited(UEventflowNode* Node, bool bSuccess, int NextNodeIndex);
	virtual void HandleOnGraphStarted();
	virtual void HandleOnGraphEnded();

};

