// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Core/PoolCollection.h"
#include "StateMachine/FiniteStateMachine.h"
#include "Type/EventflowEntry.h"

// Generated Headers
#include "EventflowEngine.generated.h"

// Forward Declarations
class UAssetManager;
class UEventflowAsset;
class UEventflowNodeTask;
struct FStreamableHandle;
struct FEventflowNode;
struct FEventflowPinRelation;


/**
 *
 */
UCLASS()
class CEVENTFLOW_API UEventflowEngine : public UFiniteStateMachine
{

	GENERATED_BODY()

public:

	virtual void InitializeData(const FPrimaryAssetId& InAssetId, const FEventflowEntryData& InEntryData);

	UEventflowNodeTask* GetTask() const;

	template<typename T>
	T* GetTask()
	{
		return Cast<T>(GetTask());
	}

	UEventflowAsset* GetAsset() const;

	template<typename T>
	T* GetAsset()
	{
		return Cast<T>(GetAsset());
	}

	const TInstancedStruct<FEventflowReturnData>& GetReturnData() const;

	// ~ UObject
	virtual UWorld* GetWorld() const override;
	// ~ End of UObject

protected:

	virtual void GetAssetBundle(TArray<FName>& OutBundle) const;
	const FEventflowNode* GetNode(const FGuid& NodeId) const;
	const FEventflowPinRelation* GetPinRelation(const FGuid& PinId) const;

	void ReachNode(const FGuid& NodeId);
	void ReachEntryNode();
	void ReachNextNode(int Index = 0);
	void ReachPreviousNode();

	void CreateTask(const FGuid& NodeId, const FEventflowNode* Node);
	void RemoveTask();
	
	void CreateReturnData(UEventflowNodeTask* Task);
	void RemoveReturnData();

	// ~ Bindings
	virtual void HandleOnTaskStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result);
	// ~ End of Bindings

	// ~ UFiniteStateMachine
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;

	virtual void OnActive(EFSMState PreviousState) override;
	virtual void OnEndActive(EFSMState NextState, EFSMResult Result) override;

	virtual void OnFinished(EFSMResult Result) override;
	virtual void OnRestart(EFSMState PreviousState, EFSMResult PreviousResult) override;
	virtual void OnReset() override;
	// ~ End of UFiniteStateMachine

private:

	TInstancedStruct<FEventflowReturnData> _ReturnData;
	FEventflowEntryData _EntryData;


	UPROPERTY()
	TMap<UClass*, FPoolCollection> _TaskPool;

	UPROPERTY()
	TObjectPtr<UEventflowNodeTask> _ActiveTask = nullptr;

	FGuid _ActiveNodeId;


	UPROPERTY()
	TObjectPtr<UEventflowAsset> _Asset = nullptr;

	FPrimaryAssetId _AssetId;

	TSharedPtr<FStreamableHandle> _AssetHandle = nullptr;

	UPROPERTY()
	TObjectPtr<UAssetManager> _AssetManager = nullptr;

};

