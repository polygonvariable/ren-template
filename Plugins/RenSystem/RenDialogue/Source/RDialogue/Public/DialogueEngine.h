// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "EventflowEngine.h"
#include "EventflowTask.h"

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

	// ~ UEventflowEngine
	virtual void StartEngine() override;
	virtual void StopEngine(bool bInterrupted) override;
	// ~ End of UEventflowEngine

	IDialogueProvider* GetDialogue() const;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	TObjectPtr<UDialogueWidget> DialogueWidget = nullptr;

};


/**
 *
 */
UCLASS(MinimalAPI, EditInlineNew, DefaultToInstanced)
class UDialogueTask : public UEventflowTask
{

	GENERATED_BODY()

public:
	
	// ~ UEventflowTask
	virtual void CopyFromTemplate(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

	virtual TArray<FText> GetRuntimeOutputs() const override;

protected:

	UPROPERTY(EditAnywhere)
	FDialogueData DialogueData2;


	IDialogueProvider* GetDialogue() const;
	void HandleDialogueCompleted(int NextIndex);

	// ~ UEventflowTask
	virtual void Initialization() override;
	virtual void Deinitialization() override;
	// ~ End of UEventflowTask

};