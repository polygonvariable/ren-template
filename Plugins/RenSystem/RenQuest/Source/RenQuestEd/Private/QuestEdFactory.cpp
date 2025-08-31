// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestEdFactory.h"

// Engine Headers
#include "Kismet2/KismetEditorUtilities.h"

// Project Headers
#include "RenQuest/Public/QuestAsset.h"



UQuestEdFactory::UQuestEdFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UQuestGraphBlueprint::StaticClass();
}

UObject* UQuestEdFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UQuestGraph* NewAsset = NewObject<UQuestGraph>(InParent, Class, Name, Flags);
    return NewAsset;
}

bool UQuestEdFactory::CanCreateNew() const
{
    return true;
}

