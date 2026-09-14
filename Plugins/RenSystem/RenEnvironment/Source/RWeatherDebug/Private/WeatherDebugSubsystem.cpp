// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherDebugSubsystem.h"


void UWeatherDebugSubsystem::HandleOnDebugCVarChanged(IConsoleVariable* Variable)
{
    if (!WeatherDebug.IsValid())
    {
        WeatherDebug = MakeUnique<FWeatherDebugWidget>();
    }

    if (Variable && WeatherDebug.IsValid())
    {
        if (Variable->GetBool())
        {
            WeatherDebug->EnableWidget();
        }
        else
        {
            WeatherDebug->DisableWidget();
        }
    }
}

bool UWeatherDebugSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    return true;
}

void UWeatherDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    IConsoleVariable* WeatherDebugCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ren.Environment.Weather.Debug"));
    if (WeatherDebugCVar)
    {
        WeatherDebugCVar->OnChangedDelegate().AddUObject(this, &UWeatherDebugSubsystem::HandleOnDebugCVarChanged);
    }
}

void UWeatherDebugSubsystem::Deinitialize()
{
    IConsoleVariable* WeatherDebugCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ren.Environment.Weather.Debug"));
    if (WeatherDebugCVar)
    {
        WeatherDebugCVar->OnChangedDelegate().Clear();
    }

    if (WeatherDebug)
    {
        WeatherDebug->DisableWidget();
    }
    WeatherDebug.Reset();

    Super::Deinitialize();
}

