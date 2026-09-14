// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "WeatherDebug.h"

// Generated Header
#include "WeatherDebugSubsystem.generated.h"

// Forward Declaration
class FWeatherDebugWidget;
class IConsoleVariable;


/*
 * 
 */
UCLASS()
class UWeatherDebugSubsystem : public UGameInstanceSubsystem
{

    GENERATED_BODY()

public:

    // ~ UGameInstanceSubsystem
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    // ~ End of UGameInstanceSubsystem

protected:

    TUniquePtr<FWeatherDebugWidget> WeatherDebug;


    // ~ Binding
    void HandleOnDebugCVarChanged(IConsoleVariable* Variable);
    // ~ End of Binding

};

