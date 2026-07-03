// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/EventflowTransition.h"
#include "Task/EventflowTask.h"

// Generated Headers
#include "EventflowPrimaryTask.generated.h"

// Module Macros
#define REN_API RENEVENTFLOW_API

// Forward Declarations
class UEventflowSubTask;
struct FEventflowNode;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UEventflowPrimaryTask : public UEventflowTask
{
	
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FEventflowTransition> TaskTransitions;

	UPROPERTY(Instanced)
	TArray<TObjectPtr<UEventflowSubTask>> SubTasks;

	UPROPERTY()
	TMap<EFSMResult, FEventflowCondition_TaskState> SubTaskConditions;


	REN_API void InitializeData(const FGuid& NodeId, const FEventflowNode* Node);

	REN_API int GetTransitionIndex(EFSMResult Result) const;
	REN_API EEventflowTransitionType GetTransitionType(EFSMResult Result) const;
	REN_API const TArray<TObjectPtr<UEventflowSubTask>>& GetSubTasks();

	// ~ UEventflowTask
	REN_API virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask
	
#if WITH_EDITOR

	// ~ UEventflowTask
	REN_API virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle) override;
	// ~ End of UEventflowTask

#endif

protected:

	REN_API void SetTransitionIndex(int Index);

	REN_API UEventflowSubTask* GetSubTask(const FName& TaskName) const;
	void CreateSubTasks();
	void RemoveSubTasks();

	// ~ Bindings
	REN_API virtual void HandleOnSubTaskStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result);
	// ~ End of Bindings

	// ~ UFiniteStateMachine
	REN_API virtual void OnInitialized(EFSMState PreviousState) override;
	REN_API virtual void OnReset() override;
	// ~ End of UFiniteStateMachine

private:

	FGuid _CurrentNodeId;

	const FEventflowNode* _CurrentNode = nullptr;

	int _TransitionIndex = 0;

	UPROPERTY()
	TArray<TObjectPtr<UEventflowSubTask>> _ActiveSubTasks;

};


// Module Macros
#undef REN_API

