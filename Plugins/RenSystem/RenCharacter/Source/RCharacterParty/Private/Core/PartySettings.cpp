// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/PartySettings.h"

// Project Header
#include "Data/PartyStorage.h"
#include "System/PartyStorageManager.h"
#include "System/PartySubsystem.h"


UPartySettings::UPartySettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	StorageId = TEXT("party001");
	StorageClass = UPartyStorage::StaticClass();
	StorageManagerClass = UPartyStorageManager::StaticClass();

	SubsystemClass = UPartySubsystem::StaticClass();

	CharacterBundles.Add(TEXT("Character"));
	CharacterBundles.Add(TEXT("Ability"));
}

const UPartySettings* UPartySettings::Get()
{
	return GetDefault<UPartySettings>();
}

