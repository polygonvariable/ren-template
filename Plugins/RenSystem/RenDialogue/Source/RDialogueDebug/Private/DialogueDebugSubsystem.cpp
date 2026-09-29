// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueDebugSubsystem.h"


void UDialogueDebugSubsystem::HandleOnDebugCVarChanged(IConsoleVariable* Variable)
{
    if (!DialogueDebug.IsValid())
    {
        DialogueDebug = MakeUnique<FDialogueDebugWidget>();
    }

    if (Variable && DialogueDebug.IsValid())
    {
        if (Variable->GetBool())
        {
            DialogueDebug->EnableWidget();
        }
        else
        {
            DialogueDebug->DisableWidget();
        }
    }
}

bool UDialogueDebugSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    return true;
}

void UDialogueDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    IConsoleVariable* DialogueDebugCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ren.Dialogue.Debug"));
    if (DialogueDebugCVar)
    {
        DialogueDebugCVar->OnChangedDelegate().AddUObject(this, &UDialogueDebugSubsystem::HandleOnDebugCVarChanged);
    }
}

void UDialogueDebugSubsystem::Deinitialize()
{
    IConsoleVariable* DialogueDebugCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ren.Dialogue.Debug"));
    if (DialogueDebugCVar)
    {
        DialogueDebugCVar->OnChangedDelegate().Clear();
    }

    if (DialogueDebug)
    {
        DialogueDebug->DisableWidget();
    }
    DialogueDebug.Reset();

    Super::Deinitialize();
}

