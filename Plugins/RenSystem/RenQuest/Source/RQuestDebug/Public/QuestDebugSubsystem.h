// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "QuestDebug.h"

// Generated Header
#include "QuestDebugSubsystem.generated.h"

// Forward Declaration
class FQuestDebugWidget;
class IConsoleVariable;


/*
 * 
 */
UCLASS()
class UQuestDebugSubsystem : public UGameInstanceSubsystem
{

    GENERATED_BODY()

public:

    // ~ UGameInstanceSubsystem
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    // ~ End of UGameInstanceSubsystem

protected:

    TUniquePtr<FQuestDebugWidget> QuestDebug;


    // ~ Binding
    void HandleOnDebugCVarChanged(IConsoleVariable* Variable);
    // ~ End of Binding

};

