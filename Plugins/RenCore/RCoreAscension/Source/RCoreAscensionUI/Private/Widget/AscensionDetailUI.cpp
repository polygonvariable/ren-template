// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AscensionDetailUI.h"

// Engine Headers
#include "Components/TextBlock.h"

// Project Headers
#include "Definition/AscensionData.h"


void UAscensionDetailUI::InitializeDetail(const FAscensionData& Data)
{
	if (RankText) RankText->SetText(FText::FromString(FString::FromInt(Data.Rank)));
	if (LevelText) LevelText->SetText(FText::FromString(FString::FromInt(Data.Level)));
	if (ExperienceText) ExperienceText->SetText(FText::FromString(FString::FromInt(Data.Experience)));
}

