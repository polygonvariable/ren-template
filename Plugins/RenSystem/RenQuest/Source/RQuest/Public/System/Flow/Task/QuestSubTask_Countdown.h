// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "System/Flow/Task/QuestSubTask.h"

// Generated Headers
#include "QuestSubTask_Countdown.generated.h"


/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Countdown"))
class UQuestTask_Countdown : public UQuestSubTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float Duration = 10.0f;


	// ~ IQuestRuntimeDataProvider
	virtual void PullQuestData(TInstancedStruct<FQuestRuntimeData>& OutData) override;
	// ~ End of IQuestRuntimeDataProvider

	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

#if UE_BUILD_DEVELOPMENT
	// ~ UEventflowTask
	virtual void GetEditorDebugInfo(TArray<FString>& OutDebug) const override;
	// ~ End of UEventflowTask
#endif

protected:

	FTimerHandle TickHandle;
	FTimerHandle TimerHandle;


	// ~ Binding
	void HandleOnTick();
	void HandleCountdownOver();
	// ~ End of Binding

	// ~ UFiniteStateMachine
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;

	virtual void OnActive(EFSMState PreviousState) override;
	virtual void OnEndActive(EFSMState NextState, EFSMResult Result) override;

	virtual void OnRestart(EFSMState PreviousState, EFSMResult PreviousResult) override;
	virtual void OnReset() override;
	// ~ End of UFiniteStateMachine

};

