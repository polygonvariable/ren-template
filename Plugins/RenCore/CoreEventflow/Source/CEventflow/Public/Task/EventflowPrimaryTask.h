// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "EventflowTask.h"
#include "LuauSourceCode.h"
#include "Type/EventflowCondition.h"
#include "Type/EventflowTransition.h"

// Generated Headers
#include "EventflowPrimaryTask.generated.h"

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
	TMap<EFSMResult, FEventflowTaskCondition> SubTaskConditions;

	UPROPERTY()
	FLuauSourceCode LuauCode;


	CEVENTFLOW_API void InitializeData(const FGuid& NodeId, const FEventflowNode* Node);

	CEVENTFLOW_API int GetTransitionIndex(EFSMResult Result) const;
	CEVENTFLOW_API void SetTransitionIndex(int Index);

	CEVENTFLOW_API EEventflowTransitionType GetTransitionType(EFSMResult Result) const;
	CEVENTFLOW_API const TArray<TObjectPtr<UEventflowSubTask>>& GetSubTasks();

	// ~ UEventflowTask
	CEVENTFLOW_API virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask
	
#if WITH_EDITOR

	// ~ UEventflowTask
	CEVENTFLOW_API virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle) override;
	// ~ End of UEventflowTask

#endif

protected:


	CEVENTFLOW_API UEventflowSubTask* GetSubTask(const FName& TaskName) const;
	void CreateSubTasks();
	void RemoveSubTasks();

	// ~ Bindings
	CEVENTFLOW_API virtual void HandleOnSubTaskStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result);
	// ~ End of Bindings

	// ~ UFiniteStateMachine
	CEVENTFLOW_API virtual void OnInitialized(EFSMState PreviousState) override;
	CEVENTFLOW_API virtual void OnReset() override;
	// ~ End of UFiniteStateMachine

private:

	FGuid _CurrentNodeId;

	const FEventflowNode* _CurrentNode = nullptr;

	int _TransitionIndex = 0;

	UPROPERTY()
	TArray<TObjectPtr<UEventflowSubTask>> _ActiveSubTasks;

};

