// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Quest/WaitForObjectiveCompletion.h"

// Engine Headers

// Project Headers
#include "RenQuest/Public/Quest/QuestInterface.h"



UWaitForObjectiveCompletion* UWaitForObjectiveCompletion::WaitForObjectiveCompletion(UObject* InTarget)
{
    UWaitForObjectiveCompletion* NewTask = NewObject<UWaitForObjectiveCompletion>();
    NewTask->Target = InTarget;
    return NewTask;
}

void UWaitForObjectiveCompletion::EndTask()
{
    IQuestObjectiveInterface* Objective = Cast<IQuestObjectiveInterface>(Target);
    if (Objective)
    {
        Objective->GetOnQuestObjectiveCompleted().Unbind();
    }

    Target.Reset();

    SetReadyToDestroy();
}

void UWaitForObjectiveCompletion::OnObjectiveCompleted()
{
    OnCompleted.Broadcast(Target.Get());
    EndTask();
}

void UWaitForObjectiveCompletion::Activate()
{
    IQuestObjectiveInterface* Objective = Cast<IQuestObjectiveInterface>(Target);
    if (!Objective)
    {
        EndTask();
        return;
    }
    
    Objective->GetOnQuestObjectiveCompleted().BindUObject(this, &UWaitForObjectiveCompletion::OnObjectiveCompleted);
}

