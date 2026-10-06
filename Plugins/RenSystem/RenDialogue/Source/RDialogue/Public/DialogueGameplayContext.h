// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "DialogueSubsystem.h"
#include "GameplayContextAction.h"

// Generated Headers
#include "DialogueGameplayContext.generated.h"


/*
 *
 */
USTRUCT(DisplayName = "Open Dialogue")
struct FGameplayContext_OpenDialogue : public FGameplayContextAction
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (AllowedTypes = "Dialogue"))
	FPrimaryAssetId DialogueAssetId;

    virtual bool Execute(UWorld* World, UObject* Caller) override
	{
        if (!IsValid(World) || !DialogueAssetId.IsValid())
        {
            return false;
        }

        UDialogueSubsystem* DialogueSubsystem = UDialogueSubsystem::Get(World);
        if (!IsValid(DialogueSubsystem))
        {
            return false;
        }
        
        DialogueSubsystem->StartDialogue(DialogueAssetId);
		return false;
	};
    
};

