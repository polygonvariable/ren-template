// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/PoolCollection.h"
#include "EventflowDefinition.h"

// Generated Headers
#include "EventflowEngine.generated.h"

// Module Macros
#define REN_API RENEVENTFLOW_API

// Forward Declarations
class UAssetManager;
class UEventflowAsset;
class UEventflowTask;
struct FStreamableHandle;
struct FEventflowNodeDefinition;
struct FEventflowPinRelation;



/**
 *
 */
UCLASS(MinimalAPI, BlueprintType, Blueprintable)
class UEventflowEngine : public UObject
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE(FEventflowEngineDelegate);
	FEventflowEngineDelegate OnStarted;
	FEventflowEngineDelegate OnEnded;


	UFUNCTION(BlueprintCallable)
	REN_API void InitializeEngine(const FPrimaryAssetId& AssetId);

	UFUNCTION(BlueprintCallable)
	REN_API void DeinitializeEngine();

	UFUNCTION(BlueprintCallable)
	REN_API virtual void StartEngine();

	UFUNCTION(BlueprintCallable)
	REN_API virtual void StopEngine(bool bInterrupted);

protected:

	FGuid CurrentLoadId;
	FPrimaryAssetId CurrentAssetId;
	FGuid CurrentNodeId;

	UPROPERTY()
	TObjectPtr<UEventflowAsset> CurrentAsset = nullptr;

	UPROPERTY()
	TObjectPtr<UEventflowTask> CurrentTask = nullptr;

	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager = nullptr;

	const FEventflowNodeDefinition* GetNode(const FGuid& NodeId) const;
	const FEventflowPinRelation* GetPinRelation(const FGuid& PinId) const;

	void ReachNode(const FGuid& NodeId);
	void ReachEntryNode();
	void ReachNextNode(int Index = 0);

	void CreateActiveTask(const FGuid& NodeId, const FEventflowNodeDefinition* Node);
	void RemoveActiveTask();
	void HandleTaskFinished(EEventflowDirection Direction, int Index);

	void HandleInitialization();
	void Fail(const FString& Message);

private:

	TSharedPtr<FStreamableHandle> _SpawnHandle;

	UPROPERTY()
	TMap<UClass*, FPoolCollection> _TaskPool;

};


// Module Macros
#undef REN_API

