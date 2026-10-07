// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameplayContextAction.h"


bool FGameplayContextAction::IsValid() const
{
    return ContextTags.IsValid();
}

bool FGameplayContextAction::Execute(UWorld* World, UObject* Owner, UObject* Instigator)
{
    return false;
}

void FGameplayContextAction::Reset()
{
    ContextTags.Reset();
}

