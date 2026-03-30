// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Settings/PartySettings.h"

// Project Headers
#include "Storage/PartyStorage.h"


UPartySettings::UPartySettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");
}

const FName& UPartySettings::GetStorageId() const
{
	return StorageId;
}

TSubclassOf<UStorage> UPartySettings::GetStorageClass() const
{
	return StorageClass;
}

const UPartySettings* UPartySettings::Get()
{
	return GetDefault<UPartySettings>();
}

