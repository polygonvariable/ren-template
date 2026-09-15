// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "SeasonDebug.h"

// Generated Header
#include "SeasonDebugSubsystem.generated.h"

// Forward Declaration
class FSeasonDebugWidget;
class IConsoleVariable;


/*
 * 
 */
UCLASS()
class USeasonDebugSubsystem : public UGameInstanceSubsystem
{

    GENERATED_BODY()

public:

    // ~ UGameInstanceSubsystem
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    // ~ End of UGameInstanceSubsystem

protected:

    TUniquePtr<FSeasonDebugWidget> SeasonDebug;


    // ~ Binding
    void HandleOnDebugCVarChanged(IConsoleVariable* Variable);
    // ~ End of Binding

};

