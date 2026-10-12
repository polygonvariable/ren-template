// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "LuauSourceCode.h"
#include "System/Flow/Task/QuestPrimaryTask.h"

// Generated Headers
#include "QuestTask_SubtaskGate.generated.h"


/**
 * 
 */
UCLASS(MinimalAPI)
class UQuestTask_SubtaskGate : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UQuestTask_SubtaskGate();

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

	UPROPERTY(EditAnywhere, Category = "Subtask Condition")
	FLuauSourceCode LuauCode;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Subtask Condition")
	bool bAutoCompile = true;
#endif

	// ~ Binding
	virtual void HandleOnSubTaskStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result) override;
	// ~ End of Bindings

	// ~ UFiniteStateMachine
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	virtual void OnFinished(EFSMResult Result) override;
	virtual void OnReset() override;
	// ~ End of UFiniteStateMachine

};

