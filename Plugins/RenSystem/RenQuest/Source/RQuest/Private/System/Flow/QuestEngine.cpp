// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Flow/QuestEngine.h"

// Project Headers
#include "Core/QuestSettings.h"


void UQuestEngine::GetAssetBundle(TArray<FName>& OutBundle) const
{
	const UQuestSettings* Settings = UQuestSettings::Get();

	OutBundle.Add(Settings->BundleName);
}

