// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "DialogueDebug.h"

// Generated Header
#include "DialogueDebugSubsystem.generated.h"

// Forward Declaration
class FDialogueDebugWidget;
class IConsoleVariable;


/*
 * 
 */
UCLASS()
class UDialogueDebugSubsystem : public UGameInstanceSubsystem
{

    GENERATED_BODY()

public:

    // ~ UGameInstanceSubsystem
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    // ~ End of UGameInstanceSubsystem

protected:

    TUniquePtr<FDialogueDebugWidget> DialogueDebug;


    // ~ Binding
    void HandleOnDebugCVarChanged(IConsoleVariable* Variable);
    // ~ End of Binding

};

