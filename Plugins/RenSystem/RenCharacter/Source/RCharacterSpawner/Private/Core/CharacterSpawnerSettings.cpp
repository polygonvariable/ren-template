// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/CharacterSpawnerSettings.h"


UCharacterSpawnerSettings::UCharacterSpawnerSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	CharacterBundles.Add(TEXT("Character"));
	CharacterBundles.Add(TEXT("Ability"));
}

const UCharacterSpawnerSettings* UCharacterSpawnerSettings::Get()
{
	return GetDefault<UCharacterSpawnerSettings>();
}

