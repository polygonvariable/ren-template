// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "DialogueData.h"
#include "Task/EventflowNodeTask.h"

// Generated Headers
#include "DialogueTask.generated.h"


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UDialoguePrimaryTask : public UEventflowNodeTask
{

	GENERATED_BODY()

public:

	UDialoguePrimaryTask();

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
class UDialogueTask_Begin : public UDialoguePrimaryTask
{

	GENERATED_BODY()

public:

	UDialogueTask_Begin();

};

/**
 *
 */
UCLASS(MinimalAPI)
class UDialogueTask_End : public UDialoguePrimaryTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Graph")
	EEventflowGraphTransitionType GraphResult = EEventflowGraphTransitionType::GraphSuccess;

	UDialogueTask_End();

};


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UDialogueTask_Base : public UDialoguePrimaryTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FDialogueSpeaker Speaker;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FDialogueData Dialogue;


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
UCLASS(MinimalAPI)
class UDialogueTask_Dialogue : public UDialogueTask_Base
{
	GENERATED_BODY()
};


/**
 *
 */
UCLASS(MinimalAPI)
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

