// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "DialogueSubsystem.h"
#include "GameplayContextAction.h"
#include "Core/EventflowIO.h"

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

    virtual bool Execute(UWorld* World, UObject* Owner, UObject* Instigator) override
	{
        if (!::IsValid(World) || !DialogueAssetId.IsValid())
        {
            return false;
        }

        UDialogueSubsystem* DialogueSubsystem = UDialogueSubsystem::Get(World);
        if (!::IsValid(DialogueSubsystem))
        {
            return false;
        }
        
        TInstancedStruct<FEventflowEntryData> EntryData;
        FEventflowEntryData& Data = EntryData.InitializeAs<FEventflowEntryData>();

        DialogueSubsystem->StartDialogue(DialogueAssetId, EntryData);
		return false;
	};
    
};

