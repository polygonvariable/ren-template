// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestAsset.h"

// Engine Headers

// Project Headers
#include "RenQuest/Public/QuestSubsystem.h"



const TArray<FText>* UQuestObjectiveNodeData::GetOutputOptions() const
{
    return &ExtraObjectives;
}


UQuestSubsystem* UQuestNodeTask::GetQuestSubsystem() const
{
    return Cast<UQuestSubsystem>(GetOuter());
}



void UQuestNodeTask_StartQuest::OnActionStarted_Implementation()
{
    UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
    if (!QuestSubsystem)
    {
		UE_LOG(LogTemp, Error, TEXT("QuestSubsystem is invalid"));
        StopAction(false);
		return;
    }

    UQuestGraph* QuestGraph = Cast<UQuestGraph>(OwnedGraphAsset.Get());
    if (!QuestGraph)
    {
        UE_LOG(LogTemp, Error, TEXT("QuestGraph is invalid"));
		StopAction(false);
		return;
    }

    if (QuestSubsystem->IsQuestActive(QuestGraph) || QuestSubsystem->IsQuestCompleted(QuestGraph))
    {
        UE_LOG(LogTemp, Warning, TEXT("Quest is already active"));
		StopAction(false);
		return;
    }

    // Do other checks like if the player has the required items, level, etc.

    UE_LOG(LogTemp, Warning, TEXT("Starting Quest: %s"), *QuestGraph->QuestTitle.ToString());
    StopAction(true);
}


void UQuestNodeTask_Objective::OnActionStarted_Implementation()
{
}


void UQuestGraphBlueprint::SetOwnedGraph(UQuestGraph* AssetGraph)
{
}
