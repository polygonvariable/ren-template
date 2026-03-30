// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Storage/PartyStorage.h"

// Project Headers
#include "Library/CharacterPrimaryAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/PartySettings.h"


void UPartyStorage::InitializeDefaults()
{
	const UPartySettings* Settings = UPartySettings::Get();

	CharacterSlot.Init(FPrimaryAssetId(), Settings->MaxPartySize);

	const TArray<FPrimaryAssetId>& DefaultCharacters = Settings->DefaultCharacters;
	int Num = DefaultCharacters.Num();
	for (int i = 0; i < Num; i++)
	{
		if (CharacterSlot.IsValidIndex(i))
		{
			CharacterSlot[i] = DefaultCharacters[i];
		}
	}
}

FVector UPartyStorage::GetPartyLocation(const FName& Level) const
{
    return PartyLocations.FindRef(Level);
}

void UPartyStorage::SetPartyLocation(const FName& Level, const FVector& Location)
{
    PartyLocations.Add(Level, Location);
}

const TArray<FPrimaryAssetId>& UPartyStorage::GetCharacters() const
{
    return CharacterSlot;
}

const TArray<FPrimaryAssetId>& UPartyStorage::GetTemporaryCharacters() const
{
    return TemporarySlot;
}

void UPartyStorage::GetAllCharacters(TArray<FPrimaryAssetId>& OutCharacters) const
{
	OutCharacters.Empty();

	for (const FPrimaryAssetId& AssetId : CharacterSlot)
	{
		if (AssetId.IsValid())
		{
			OutCharacters.Add(AssetId);
		}
	}
	for (const FPrimaryAssetId& AssetId : TemporarySlot)
	{
		if (AssetId.IsValid())
		{
			OutCharacters.Add(AssetId);
		}
	}
}

bool UPartyStorage::GetCharacterAtSlot(int Slot, FPrimaryAssetId& AssetId) const
{
	if (!CharacterSlot.IsValidIndex(Slot) || !CharacterSlot[Slot].IsValid())
	{
		AssetId = FPrimaryAssetId();
		return false;
	}

	AssetId = CharacterSlot[Slot];
	return true;
}

bool UPartyStorage::SetCharacterAtSlot(int Slot, FPrimaryAssetId AssetId)
{
	if (!FCharacterPrimaryAsset::IsValid(AssetId))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Asset type is not character"));
		return false;
	}

	if (!CharacterSlot.IsValidIndex(Slot))
	{
		return false;
	}

	int32 ExistingSlot = CharacterSlot.IndexOfByKey(AssetId);
	if (ExistingSlot != INDEX_NONE)
	{
		CharacterSlot[ExistingSlot] = FPrimaryAssetId();
	}

	CharacterSlot[Slot] = AssetId;
	OnStorageUpdated.Broadcast();

	return true;
}

bool UPartyStorage::RemoveCharacterFromSlot(int Slot)
{
	if (!CharacterSlot.IsValidIndex(Slot))
	{
		return false;
	}

	int NumValidCharacters = 0;
	for (int i = 0; i < CharacterSlot.Num(); i++)
	{
		if (CharacterSlot[i].IsValid())
		{
			NumValidCharacters++;
		}
	}

	if (NumValidCharacters == UPartySettings::Get()->MinPartySize)
	{
		return false;
	}

	CharacterSlot[Slot] = FPrimaryAssetId();
	OnStorageUpdated.Broadcast();
	return true;
}

