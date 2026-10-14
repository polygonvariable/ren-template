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

protected:

	UPROPERTY(EditAnywhere, Category = "Global Task", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FObjectiveGlobalTaskFragment>> GlobalTasks;

	// ~ UEventflowTask
	virtual void OnActive(EFSMState PreviousState) override;
	// ~ End of UEventflowTask

};

