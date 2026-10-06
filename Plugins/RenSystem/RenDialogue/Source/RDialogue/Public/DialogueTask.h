// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "DialogueData.h"
#include "Task/EventflowPrimaryTask.h"

// Generated Headers
#include "DialogueTask.generated.h"


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UDialogueTask_Base : public UEventflowPrimaryTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FDialogueSpeaker Speaker;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FDialogueData Dialogue;

	int NextIndex = 0;


	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

protected:

	// ~ UEventflowTask
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	virtual void OnFinished(EFSMResult Result) override;
	// ~ End of UEventflowTask

};


/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Dialogue"))
class UDialogueTask_Default : public UDialogueTask_Base
{
	GENERATED_BODY()
};


/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Dialogue Branch"))
class UDialogueTask_Branch : public UDialogueTask_Base
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Dialogue Options")
	TArray<FText> Options;

	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

protected:

	// ~ UEventflowTask
	virtual void OnActive(EFSMState PreviousState) override;
	virtual void OnFinished(EFSMResult Result) override;
	// ~ End of UEventflowTask

};

