// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "SaveGame/Storage.h"

// Generated Headers
#include "PartyStorage.generated.h"

// Module Macros
#define REN_API RCHARACTERPARTY_API


/**
 *
 */
UCLASS(MinimalAPI)
class UPartyStorage : public UStorage
{

	GENERATED_BODY()

public:

	REN_API FVector GetPartyLocation(const FName& Level) const;
	REN_API void SetPartyLocation(const FName& Level, const FVector& Location);

	REN_API const TArray<FPrimaryAssetId>& GetCharacters() const;
	REN_API const TArray<FPrimaryAssetId>& GetTemporaryCharacters() const;
	REN_API void GetAllCharacters(TArray<FPrimaryAssetId>& OutCharacters) const;

	REN_API bool GetCharacterAtSlot(int Slot, FPrimaryAssetId& AssetId) const;
	REN_API bool SetCharacterAtSlot(int Slot, FPrimaryAssetId AssetId);
	REN_API bool RemoveCharacterFromSlot(int Slot);

	// ~ UStorage
	virtual void InitializeDefaults() override;
	// ~ End of UStorage

protected:

	/* Party Locations on different levels */
	UPROPERTY(SaveGame)
	TMap<FName, FVector> PartyLocations;

	UPROPERTY(SaveGame)
	TArray<FPrimaryAssetId> CharacterSlot;

	UPROPERTY(Transient)
	TArray<FPrimaryAssetId> TemporarySlot;

};


// Module Macros
#undef REN_API

