// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/SaveGame.h"

// Project Headers
#include "Definition/Runtime/InventoryStack.h"

// Generated Headers
#include "InventoryStorage.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UInventoryStorage : public USaveGame
{

	GENERATED_BODY()

public:

	UPROPERTY(SaveGame)
	TMap<FPrimaryAssetId, FInventoryStack> InventoryStack;

};

