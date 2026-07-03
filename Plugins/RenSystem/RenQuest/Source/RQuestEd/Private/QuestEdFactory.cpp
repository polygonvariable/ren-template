// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestEdFactory.h"

// Project Headers
#include "Data/QuestAsset.h"


UQuestEdFactory::UQuestEdFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UQuestAsset::StaticClass();
}

UObject* UQuestEdFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
    return NewObject<UQuestAsset>(InParent, InClass, InName, Flags);
}

bool UQuestEdFactory::CanCreateNew() const
{
    return true;
}

