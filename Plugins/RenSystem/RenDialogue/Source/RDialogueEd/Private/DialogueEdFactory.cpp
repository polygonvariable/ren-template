// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueEdFactory.h"

// Project Headers
#include "DialogueAsset.h"


UDialogueEdFactory::UDialogueEdFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UDialogueAsset::StaticClass();
}

UObject* UDialogueEdFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
    return NewObject<UDialogueAsset>(InParent, InClass, InName, Flags);
}

bool UDialogueEdFactory::CanCreateNew() const
{
    return true;
}

