// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "SeasonDebugSubsystem.h"


void USeasonDebugSubsystem::HandleOnDebugCVarChanged(IConsoleVariable* Variable)
{
    if (!SeasonDebug.IsValid())
    {
        SeasonDebug = MakeUnique<FSeasonDebugWidget>();
    }

    if (Variable && SeasonDebug.IsValid())
    {
        if (Variable->GetBool())
        {
            SeasonDebug->EnableWidget();
        }
        else
        {
            SeasonDebug->DisableWidget();
        }
    }
}

bool USeasonDebugSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    return true;
}

void USeasonDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    IConsoleVariable* SeasonDebugCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ren.Environment.Season.Debug"));
    if (SeasonDebugCVar)
    {
        SeasonDebugCVar->OnChangedDelegate().AddUObject(this, &USeasonDebugSubsystem::HandleOnDebugCVarChanged);
    }
}

void USeasonDebugSubsystem::Deinitialize()
{
    IConsoleVariable* SeasonDebugCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ren.Environment.Season.Debug"));
    if (SeasonDebugCVar)
    {
        SeasonDebugCVar->OnChangedDelegate().Clear();
    }

    if (SeasonDebug)
    {
        SeasonDebug->DisableWidget();
    }
    SeasonDebug.Reset();

    Super::Deinitialize();
}

