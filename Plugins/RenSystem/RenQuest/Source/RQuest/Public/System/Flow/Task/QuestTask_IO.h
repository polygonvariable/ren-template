// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "System/Flow/Task/QuestPrimaryTask.h"

// Generated Headers
#include "QuestTask_IO.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_Begin : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UQuestTask_Begin();

protected:

	// ~ UEventflowTask
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	// ~ End of UEventflowTask

};

/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_End : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UQuestTask_End();

	UPROPERTY(EditAnywhere, Category = "Graph")
	EEventflowGraphTransitionType GraphResult = EEventflowGraphTransitionType::GraphSuccess;

	// ~ UEventflowTask
	virtual void GetReturnData(TInstancedStruct<FEventflowReturnData>& ReturnData) override;
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

protected:

	// ~ UEventflowTask
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	// ~ End of UEventflowTask

};

