// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/PartyStorageManager.h"

// Project Headers
#include "Core/CharacterPrimaryAsset.h"
#include "Core/PartySettings.h"
#include "Data/PartyStorage.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


FVector UPartyStorageManager::GetPartyLocation(const FName& Level) const
{
	if (!IsValid(LocalStorage))
	{
		return FVector();
	}
	return LocalStorage->PartyLocations.FindRef(Level);
}

void UPartyStorageManager::SetPartyLocation(const FName& Level, const FVector& Location)
{
	if (!IsValid(LocalStorage))
	{
		return;
	}
	LocalStorage->PartyLocations.Add(Level, Location);
}

TArray<FPrimaryAssetId> UPartyStorageManager::GetCharacters() const
{
	if (!IsValid(LocalStorage))
	{
		return TArray<FPrimaryAssetId>();
	}
	return LocalStorage->CharacterSlot;
}

TArray<FPrimaryAssetId> UPartyStorageManager::GetTemporaryCharacters() const
{
	if (!IsValid(LocalStorage))
	{
		return TArray<FPrimaryAssetId>();
	}
	return LocalStorage->TemporarySlot;
}

void UPartyStorageManager::GetAllCharacters(TArray<FPrimaryAssetId>& OutCharacters) const
{
	OutCharacters.Empty();

	if (!IsValid(LocalStorage))
	{
		return;
	}

	const TArray<FPrimaryAssetId>& CharacterSlot = LocalStorage->CharacterSlot;
	const TArray<FPrimaryAssetId>& TemporarySlot = LocalStorage->TemporarySlot;

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

bool UPartyStorageManager::GetCharacterAtSlot(int Slot, FPrimaryAssetId& AssetId) const
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}

	const TArray<FPrimaryAssetId>& CharacterSlot = LocalStorage->CharacterSlot;
	if (!CharacterSlot.IsValidIndex(Slot) || !CharacterSlot[Slot].IsValid())
	{
		AssetId = FPrimaryAssetId();
		return false;
	}

	AssetId = CharacterSlot[Slot];
	return true;
}

bool UPartyStorageManager::SetCharacterAtSlot(int Slot, FPrimaryAssetId AssetId)
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}

	if (!FCharacterPrimaryAsset::IsValid(AssetId))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Asset type is not character"));
		return false;
	}

	TArray<FPrimaryAssetId>& CharacterSlot = LocalStorage->CharacterSlot;
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

bool UPartyStorageManager::RemoveCharacterFromSlot(int Slot)
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}

	TArray<FPrimaryAssetId>& CharacterSlot = LocalStorage->CharacterSlot;
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

UObject* UPartyStorageManager::GetStorage() const
{
	return LocalStorage;
}

void UPartyStorageManager::SetStorage(UObject* Storage)
{
	LocalStorage = Cast<UPartyStorage>(Storage);
}

void UPartyStorageManager::OnStorageLoaded(bool bIsNew)
{
	if (IsValid(LocalStorage) && bIsNew)
	{
		const UPartySettings* Settings = UPartySettings::Get();

		TArray<FPrimaryAssetId>& CharacterSlot = LocalStorage->CharacterSlot;

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
}

FGameEventDelegate& UPartyStorageManager::GetOnStorageUpdated()
{
	return OnStorageUpdated;
}

bool UPartyStorageManager::HasLinkedInstance(const FPrimaryAssetId& AssetId, const FGuid& AssetInstanceId) const
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}
	return LocalStorage->CharacterSlot.Contains(AssetId);
}

