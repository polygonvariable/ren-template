// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "EventflowEngine.h"
#include "Task/EventflowPrimaryTask.h"

#include "DialogueNodeData.h"

// Generated Headers
#include "DialogueEngine.generated.h"

// Forward Declarations
class IDialogueProvider;
class UDialogueWidget;


/**
 *
 */
UCLASS(MinimalAPI)
class UDialogueEngine : public UEventflowEngine
{

	GENERATED_BODY()

public:

	IDialogueProvider* GetDialogue() const;

protected:

	UPROPERTY()
	TObjectPtr<UDialogueWidget> DialogueWidget = nullptr;


	// ~ UEventflowEngine
	//virtual bool Initialization() override;
	//virtual void Deinitialization() override;
	// ~ End of UEventflowEngine

};


/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Dialogue"))
class UDialogueTask : public UEventflowPrimaryTask
{

	GENERATED_BODY()

public:
	
	// ~ UEventflowTask
	//virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

protected:

	UPROPERTY(EditAnywhere)
	FDialogueData DialogueData;


	IDialogueProvider* GetDialogue() const;

	// ~ Binding
	void HandleDialogueCompleted(int NextIndex);
	// ~ End of Binding

	// ~ UEventflowTask
	//virtual void Initialization() override;
	//virtual void Deinitialization() override;
	// ~ End of UEventflowTask

};