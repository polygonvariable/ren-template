// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueEdFactory.h"

// Engine Headers

// Project Headers
#include "RenDialogue/Public/DialogueAsset.h"



UDialogueEdFactory::UDialogueEdFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UDialogueAsset::StaticClass();
}

UObject* UDialogueEdFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UDialogueAsset* NewAsset = NewObject<UDialogueAsset>(InParent, Class, Name, Flags);
	return NewAsset;
}

bool UDialogueEdFactory::CanCreateNew() const
{
    return true;
}

