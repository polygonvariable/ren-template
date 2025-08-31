// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueEdFactory.h"

// Engine Headers
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

// Project Headers
#include "RenDialogue/Public/DialogueAsset.h"



UDialogueEdFactory::UDialogueEdFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UDialogueAsset::StaticClass();
}

UObject* UDialogueEdFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
    UDialogueAsset* NewAsset = NewObject<UDialogueAsset>(InParent, InClass, InName, Flags);
    return NewAsset;
}

bool UDialogueEdFactory::CanCreateNew() const
{
    return true;
}

