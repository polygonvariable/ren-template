// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Quest/QuestActor.h"

// Engine Headers

// Project Headers
#include "RenEventflow/Public/EventflowNode.h"



FOnQuestObjectiveCompleted& AQuestActor::GetOnQuestObjectiveCompleted()
{
    return OnQuestObjectiveCompleted;
}

void AQuestActor::InitializeObjective()
{
    Execute_BP_InitializeObjective(this);
}

void AQuestActor::CompleteObjective()
{
    OnQuestObjectiveCompleted.ExecuteIfBound();
}

void AQuestActor::DestroyObjective()
{
    ObjectiveNode.Reset();
    OnQuestObjectiveCompleted.Unbind();

    Execute_BP_DestroyObjective(this);
}

void AQuestActor::SetObjectiveNode(UEventflowNode* Node)
{
    ObjectiveNode = Node;
}

UEventflowNode* AQuestActor::GetObjectiveNode()
{
    return ObjectiveNode.Get();
}

void AQuestActor::BP_InitializeObjective_Implementation()
{
}

void AQuestActor::BP_CompleteObjective()
{
    CompleteObjective();
}

void AQuestActor::BP_DestroyObjective_Implementation()
{
}