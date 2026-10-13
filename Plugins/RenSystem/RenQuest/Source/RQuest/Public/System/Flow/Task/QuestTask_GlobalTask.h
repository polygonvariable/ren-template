// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/ObjectiveGlobalTaskFragment.h"
#include "System/Flow/Task/QuestPrimaryTask.h"

// Generated Headers
#include "QuestTask_GlobalTask.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_EnsureGlobal : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Global Task", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FObjectiveGlobalTaskFragment>> GlobalTasks;

protected:

	// ~ UEventflowTask
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	// ~ End of UEventflowTask

};

