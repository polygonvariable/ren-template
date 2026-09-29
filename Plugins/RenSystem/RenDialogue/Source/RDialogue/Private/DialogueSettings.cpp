// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueSettings.h"


UDialogueSettings::UDialogueSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	DialogueMode = TEXT("Dialogue");
}

const UDialogueSettings* UDialogueSettings::Get()
{
	return GetDefault<UDialogueSettings>();
}

