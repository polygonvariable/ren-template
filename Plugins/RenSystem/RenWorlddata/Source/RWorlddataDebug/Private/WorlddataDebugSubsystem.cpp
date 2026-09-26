// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WorlddataDebugSubsystem.h"


void UWorlddataDebugSubsystem::ToggleWidgetVisiblity(FSlateIMWidgetBase* Widget, IConsoleVariable* CVar)
{
    if (CVar->GetBool())
    {
        Widget->EnableWidget();
    }
    else
    {
        Widget->DisableWidget();
    }
}

void UWorlddataDebugSubsystem::HandleOnDebugCVarChanged(IConsoleVariable* Variable)
{
    if (Variable == ClockDebugCVar)
    {
        if (!ClockDebug.IsValid())
        {
            ClockDebug = MakeUnique<FClockDebugWidget>();
        }
        ToggleWidgetVisiblity(ClockDebug.Get(), ClockDebugCVar);
    }
}

bool UWorlddataDebugSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    return true;
}

void UWorlddataDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    ClockDebugCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ren.Gamedata.Clock.Debug"));
    if (ClockDebugCVar)
    {
        ClockDebugCVar->OnChangedDelegate().AddUObject(this, &UWorlddataDebugSubsystem::HandleOnDebugCVarChanged);
    }
}

void UWorlddataDebugSubsystem::Deinitialize()
{
    if (ClockDebugCVar)
    {
        ClockDebugCVar->OnChangedDelegate().Clear();
    }

    if (ClockDebug)
    {
        ClockDebug->DisableWidget();
    }
    ClockDebug.Reset();

    Super::Deinitialize();
}

