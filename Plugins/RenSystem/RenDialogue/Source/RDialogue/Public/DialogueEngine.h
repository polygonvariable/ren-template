// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "DialogueData.h"
#include "EventflowEngine.h"

// Generated Headers
#include "DialogueEngine.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UDialogueEngine : public UEventflowEngine
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE_TwoParams(FOnDialogueContentUpdated, const FDialogueData& /* Dialogue */, const FDialogueSpeaker& /* Speaker */);
	FOnDialogueContentUpdated OnDialogueContentUpdated;

	DECLARE_DELEGATE_OneParam(FOnDialogueOptionsUpdated, const TArray<FText>& /* Options */);
	FOnDialogueOptionsUpdated OnDialogueOptionsUpdated;

	DECLARE_DELEGATE(FOnDialogueRemoved);
	FOnDialogueRemoved OnDialogueRemoved;


	RDIALOGUE_API void SkipDialogue();
	RDIALOGUE_API void NextDialogue(int Index);

protected:

	// ~ UEventflowEngine
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnReset() override;
	// ~ End of UEventflowEngine

};

