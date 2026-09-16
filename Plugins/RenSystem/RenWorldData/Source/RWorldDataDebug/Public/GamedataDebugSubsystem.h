// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "ClockDebug.h"

// Generated Header
#include "GamedataDebugSubsystem.generated.h"

// Forward Declaration
class FSlateIMWidgetBase;
class IConsoleVariable;


/*
 * 
 */
UCLASS()
class UGamedataDebugSubsystem : public UGameInstanceSubsystem
{

    GENERATED_BODY()

public:

    // ~ UGameInstanceSubsystem
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    // ~ End of UGameInstanceSubsystem

protected:

    IConsoleVariable* ClockDebugCVar;
    TUniquePtr<FClockDebugWidget> ClockDebug;


    void ToggleWidgetVisiblity(FSlateIMWidgetBase* Widget, IConsoleVariable* CVar);


    // ~ Binding
    void HandleOnDebugCVarChanged(IConsoleVariable* Variable);
    // ~ End of Binding

};

