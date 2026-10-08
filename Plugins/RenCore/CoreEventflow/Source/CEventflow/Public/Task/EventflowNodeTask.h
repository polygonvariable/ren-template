// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "EventflowTask.h"
#include "Type/EventflowTransition.h"
#include "Type/EventflowEntry.h"

// Generated Headers
#include "EventflowNodeTask.generated.h"

// Forward Declarations
class UEventflowSubTask;
struct FEventflowNode;


/**
 *
 */
UCLASS(Abstract)
class CEVENTFLOW_API UEventflowNodeTask : public UEventflowTask
{
	
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Instanced, Category = "Node", meta = (EditCondition = "bAllowSubTasks"))
	TArray<TObjectPtr<UEventflowSubTask>> SubTasks;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay, Category = "Node")
	EEventflowNodeType NodeType = EEventflowNodeType::Other;


	void InitializeData(const FGuid& NodeId, const FEventflowNode* Node);

	virtual void GetReturnData(TInstancedStruct<FEventflowReturnData>& ReturnData);
	virtual TInstancedStruct<FEventflowNodeTransitionData>& GetTransitionData(EFSMResult Result);
	virtual void ModifyTransitionData(TFunctionRef<void(TInstancedStruct<FEventflowNodeTransitionData>&)> TransitionData);

	const TArray<TObjectPtr<UEventflowSubTask>>& GetSubTasks();
	UEventflowSubTask* GetSubTask(const FName& InTaskName) const;

#if WITH_EDITOR
	// ~ UEventflowTask
	virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle) override;
	// ~ End of UEventflowTask

	// ~ UObject
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UObject
#endif

protected:

	UPROPERTY(VisibleAnywhere, AdvancedDisplay, Category = "Node")
	bool bAllowSubTasks = false;


	virtual void CreateTransitionData();
	virtual void RemoveTransitionData();

	FGuid GetOwningNodeId() const;
	const FEventflowNode* GetOwningNode() const;
	UEventflowTask* GetOwningTemplate() const;

	template<typename T>
	T* GetOwningTemplate() const
	{
		return Cast<T>(GetOwningTemplate());
	}

	void CreateSubTasks();
	void RemoveSubTasks();

	// ~ Bindings
	virtual void HandleOnSubTaskStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result);
	// ~ End of Bindings

	// ~ UFiniteStateMachine
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnReset() override;
	// ~ End of UFiniteStateMachine

private:

	FGuid _OwningNodeId;

	const FEventflowNode* _OwningNode = nullptr;

	UPROPERTY()
	TInstancedStruct<FEventflowNodeTransitionData> _TransitionData;

	UPROPERTY()
	TArray<TObjectPtr<UEventflowSubTask>> _ActiveSubTasks;

};

