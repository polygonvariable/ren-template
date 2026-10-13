// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "System/Flow/Task/QuestPrimaryTask.h"

// Generated Headers
#include "QuestTask_Storage.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_CheckStorage : public UQuestPrimaryTask
{

	GENERATED_BODY()

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
class UQuestTask_Checkpoint : public UQuestPrimaryTask
{

	GENERATED_BODY()

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
class UQuestTask_Complete : public UQuestPrimaryTask
{

	GENERATED_BODY()

protected:

	// ~ UEventflowTask
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	// ~ End of UEventflowTask

};

