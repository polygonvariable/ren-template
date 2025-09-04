// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestEdFactory.h"

// Engine Headers
#include "Kismet2/KismetEditorUtilities.h"

// Project Headers
#include "RenQuest/Public/Quest/QuestAsset.h"



UQuestEdFactory::UQuestEdFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UQuestAsset::StaticClass();
}

UObject* UQuestEdFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UQuestAsset* NewAsset = NewObject<UQuestAsset>(InParent, Class, Name, Flags);
    return NewAsset;
}

bool UQuestEdFactory::CanCreateNew() const
{
    return true;
}

