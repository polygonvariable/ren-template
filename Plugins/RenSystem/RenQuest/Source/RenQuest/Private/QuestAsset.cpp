// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "QuestAsset.h"

// Engine Headers

// Project Headers



const TArray<FText>* UQuestObjectiveNodeData::GetOutputOptions() const
{
    return &ExtraObjectives;
}

