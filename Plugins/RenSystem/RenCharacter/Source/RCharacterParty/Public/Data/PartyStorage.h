// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/SaveGame.h"

// Generated Headers
#include "PartyStorage.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UPartyStorage : public USaveGame
{

	GENERATED_BODY()

public:

	/* Party Locations on different levels */
	UPROPERTY(SaveGame)
	TMap<FName, FVector> PartyLocations;

	UPROPERTY(SaveGame)
	TArray<FPrimaryAssetId> CharacterSlot;

	UPROPERTY(Transient)
	TArray<FPrimaryAssetId> TemporarySlot;

};

