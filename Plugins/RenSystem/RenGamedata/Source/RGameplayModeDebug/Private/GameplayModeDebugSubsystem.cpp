// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "GameplayModeDebugSubsystem.h"


void UGameplayModeDebugSubsystem::HandleOnDebugCVarChanged(IConsoleVariable* Variable)
{
    if (!GameplayModeDebug.IsValid())
    {
        GameplayModeDebug = MakeUnique<FGameplayModeDebugWidget>();
    }

    if (Variable && GameplayModeDebug.IsValid())
    {
        if (Variable->GetBool())
        {
            GameplayModeDebug->EnableWidget();
        }
        else
        {
            GameplayModeDebug->DisableWidget();
        }
    }
}

bool UGameplayModeDebugSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    return true;
}

void UGameplayModeDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    IConsoleVariable* GameplayModeDebugCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ren.GameplayMode.Debug"));
    if (GameplayModeDebugCVar)
    {
        GameplayModeDebugCVar->OnChangedDelegate().AddUObject(this, &UGameplayModeDebugSubsystem::HandleOnDebugCVarChanged);
    }
}

void UGameplayModeDebugSubsystem::Deinitialize()
{
    IConsoleVariable* GameplayModeDebugCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ren.GameplayMode.Debug"));
    if (GameplayModeDebugCVar)
    {
        GameplayModeDebugCVar->OnChangedDelegate().Clear();
    }

    if (GameplayModeDebug)
    {
        GameplayModeDebug->DisableWidget();
    }
    GameplayModeDebug.Reset();

    Super::Deinitialize();
}

