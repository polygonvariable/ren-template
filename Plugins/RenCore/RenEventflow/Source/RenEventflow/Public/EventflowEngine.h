// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/PoolCollection.h"
#include "StateMachine/FiniteStateMachine.h"

// Generated Headers
#include "EventflowEngine.generated.h"

// Module Macros
#define REN_API RENEVENTFLOW_API

// Forward Declarations
class UAssetManager;
class UEventflowAsset;
class UEventflowPrimaryTask;
struct FStreamableHandle;
struct FEventflowNode;
struct FEventflowPinRelation;


/**
 *
 */
UENUM(BlueprintType)
enum class EEventflowEntryLocation : uint8
{
	Root UMETA(DisplayName = "Root"),
	Custom UMETA(DisplayName = "Custom")
};


/**
 *
 */
USTRUCT(BlueprintType)
struct FEventflowEntryDefinition
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bAutoStart==true", EditConditionHides))
	EEventflowEntryLocation EntryLocation = EEventflowEntryLocation::Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "EntryLocation==EEventflowEntryLocation::Custom", EditConditionHides))
	FGuid NodeId = FGuid();

	void Reset()
	{
		EntryLocation = EEventflowEntryLocation::Root;
		NodeId = FGuid();
	}

};


/**
 *
 */
UCLASS(MinimalAPI, BlueprintType, Blueprintable)
class UEventflowEngine : public UFiniteStateMachine
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	REN_API virtual void InitializeData(const FPrimaryAssetId& AssetId, const FEventflowEntryDefinition& EntryDefinition);

	// ~ UObject
	REN_API virtual UWorld* GetWorld() const override;
	// ~ End of UObject

	REN_API UEventflowPrimaryTask* GetTask() const;

	template<typename T>
	T* GetTask()
	{
		return Cast<T>(GetTask());
	}

	REN_API UEventflowAsset* GetAsset() const;

	template<typename T>
	T* GetAsset()
	{
		return Cast<T>(GetAsset());
	}

protected:

	REN_API virtual void GetAssetBundle(TArray<FName>& OutBundle) const;

	REN_API const FEventflowNode* GetNode(const FGuid& NodeId) const;
	REN_API const FEventflowPinRelation* GetPinRelation(const FGuid& PinId) const;

	REN_API void ReachNode(const FGuid& NodeId);
	REN_API void ReachEntryNode();
	REN_API void ReachNextNode(int Index = 0);
	REN_API void ReachPreviousNode();

	REN_API void CreateTask(const FGuid& NodeId, const FEventflowNode* Node);
	REN_API void RemoveTask();

	// ~ Bindings
	REN_API virtual void HandleOnTaskStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result);
	// ~ End of Bindings

	// ~ UFiniteStateMachine
	REN_API virtual void OnInitialized(EFSMState PreviousState) override;
	REN_API virtual void OnLoaded(EFSMState PreviousState) override;
	REN_API virtual void OnReady(EFSMState PreviousState) override;

	REN_API virtual void OnActive(EFSMState PreviousState) override;
	REN_API virtual void OnEndActive(EFSMState NextState, EFSMResult Result) override;

	REN_API virtual void OnFinished(EFSMResult Result) override;
	REN_API virtual void OnRestart(EFSMState PreviousState, EFSMResult PreviousResult) override;
	REN_API virtual void OnReset() override;
	// ~ End of UFiniteStateMachine

private:

	FEventflowEntryDefinition _EntryDefinition;

	UPROPERTY()
	TMap<UClass*, FPoolCollection> _TaskPool;

	UPROPERTY()
	TObjectPtr<UEventflowPrimaryTask> _ActiveTask = nullptr;

	FGuid _ActiveNodeId;

	FPrimaryAssetId _AssetId;

	TSharedPtr<FStreamableHandle> _AssetHandle = nullptr;

	UPROPERTY()
	TObjectPtr<UEventflowAsset> _Asset = nullptr;

	UPROPERTY()
	TObjectPtr<UAssetManager> _AssetManager = nullptr;

};


// Module Macros
#undef REN_API

