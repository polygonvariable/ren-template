// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "EventflowTask.h"
#include "LuauSourceCode.h"
#include "Type/EventflowTransition.h"
#include "Type/EventflowEntry.h"

// Generated Headers
#include "EventflowPrimaryTask.generated.h"

// Forward Declarations
class UEventflowSubTask;
struct FEventflowNode;


/**
 *
 */
UCLASS(Abstract)
class CEVENTFLOW_API UEventflowPrimaryTask : public UEventflowTask
{
	
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Task")
	FName TaskTitle;

	UPROPERTY(EditAnywhere, Instanced, Category = "Task", meta = (EditCondition = "bAllowSubTasks"))
	TArray<TObjectPtr<UEventflowSubTask>> SubTasks;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay, Category = "Task")
	EEventflowPrimaryTaskType TaskType = EEventflowPrimaryTaskType::Other;


	void InitializeData(const FGuid& NodeId, const FEventflowNode* Node);


	virtual void GetReturnData(TInstancedStruct<FEventflowReturnData>& ReturnData);
	virtual TInstancedStruct<FEventflowTransitionData>& GetTransitionData(EFSMResult Result);
	virtual void ModifyTransitionData(TFunctionRef<void(TInstancedStruct<FEventflowTransitionData>&)> TransitionData);


	const TArray<TObjectPtr<UEventflowSubTask>>& GetSubTasks();
	UEventflowSubTask* GetSubTask(const FName& TaskName) const;

	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask
	
#if WITH_EDITOR
	// ~ UEventflowTask
	virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle) override;
	// ~ End of UEventflowTask
#endif

protected:

	UPROPERTY(VisibleAnywhere, AdvancedDisplay, Category = "Task")
	bool bAllowSubTasks = false;


	virtual void CreateTransitionData();
	virtual void RemoveTransitionData();

	FGuid GetOwningNodeId() const;
	const FEventflowNode* GetOwningNode() const;
	const UEventflowTask* GetOwningTemplate() const;

	template<typename T>
	const T* GetOwningTemplate() const
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
	TInstancedStruct<FEventflowTransitionData> _TransitionData;

	UPROPERTY()
	TArray<TObjectPtr<UEventflowSubTask>> _ActiveSubTasks;

};

