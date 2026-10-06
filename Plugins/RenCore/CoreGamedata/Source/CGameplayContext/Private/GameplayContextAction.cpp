// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameplayContextAction.h"


bool FGameplayContextAction::IsDataValid() const
{
    return ContextTags.IsValid();
}

bool FGameplayContextAction::Execute(UWorld* World, UObject* Caller)
{
    return false;
}

void FGameplayContextAction::Reset()
{
    ContextTags.Reset();
}

