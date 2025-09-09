// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "EventflowEngine.generated.h"

// Forward Declarations
class UEventflowAsset;
class UEventflowNode;
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

	UEventflowAsset* GetOwningAsset() const;

	void LoadAsset(TSoftObjectPtr<UEventflowAsset> InEventflowAsset);
	void LoadAsset(UEventflowAsset* InEventflowAsset);

	void UnloadAsset();

	void ReachEntryNode();
	void ReachNode(FGuid NodeID);
	void ReachNextNode(int Index);
	void ReachImmediateNextNode();

	virtual void ConstructBlueprint(TSubclassOf<UObject> InClass);
	virtual void DestructBlueprint();

	UEventflowNode* GetCurrentNode();
	UEventflowBlueprint* GetCurrentBlueprint();

	bool ExecuteNode(UEventflowNode* Node);

	DECLARE_DELEGATE_OneParam(FOnNodeReached, UEventflowNode*);
	FOnNodeReached OnNodeReached;

	DECLARE_DELEGATE_TwoParams(FOnNodeExited, UEventflowNode*, bool);
	FOnNodeExited OnNodeExited;

	DECLARE_DELEGATE(FOnGraphEnded);
	FOnGraphEnded OnGraphEnded;

	DECLARE_DELEGATE(FOnEngineInitialized);
	FOnEngineInitialized OnEngineInitialized;

protected:

	UEventflowBlueprint* CurrentBlueprint;

	UPROPERTY()
	TSoftObjectPtr<UEventflowAsset> CurrentAssetPath;

	UPROPERTY()
	TObjectPtr<UEventflowAsset> CurrentAsset;

	UPROPERTY()
	TObjectPtr<UEventflowNode> CurrentNode;


	void InitializeEngine();

	void SetCurrentNode(UEventflowNode* Node);

	virtual void HandleAssetLoaded();

	virtual void HandleOnNodeReached(UEventflowNode* Node);
	virtual void HandleOnNodeExited(UEventflowNode* Node, bool bSuccess);
	virtual void HandleOnGraphEnded();
	virtual void HandleOnEngineInitialized();

};

