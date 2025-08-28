// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "StoryboardAsset.h"

// Engine Headers

// Project Headers



const TArray<FText>* UStoryboardQuestData::GetOutputOptions() const
{
    return &ExtraQuests;
}

