// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestDebugSubsystem.h"


void UQuestDebugSubsystem::HandleOnDebugCVarChanged(IConsoleVariable* Variable)
{
    if (!QuestDebug.IsValid())
    {
        QuestDebug = MakeUnique<FQuestDebugWidget>();
    }

    if (Variable && QuestDebug.IsValid())
    {
        if (Variable->GetBool())
        {
            QuestDebug->EnableWidget();
        }
        else
        {
            QuestDebug->DisableWidget();
        }
    }
}

bool UQuestDebugSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    return true;
}

void UQuestDebugSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    IConsoleVariable* QuestDebugCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ren.Quest.Debug"));
    if (QuestDebugCVar)
    {
        QuestDebugCVar->OnChangedDelegate().AddUObject(this, &UQuestDebugSubsystem::HandleOnDebugCVarChanged);
    }
}

void UQuestDebugSubsystem::Deinitialize()
{
    IConsoleVariable* QuestDebugCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ren.Quest.Debug"));
    if (QuestDebugCVar)
    {
        QuestDebugCVar->OnChangedDelegate().Clear();
    }

    if (QuestDebug)
    {
        QuestDebug->DisableWidget();
    }
    QuestDebug.Reset();

    Super::Deinitialize();
}

