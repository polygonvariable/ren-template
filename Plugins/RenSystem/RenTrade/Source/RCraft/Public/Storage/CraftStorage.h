// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/SaveGame.h"

// Project Headers
#include "Definition/Runtime/CraftData.h"
#include "Definition/Runtime/TradeKey.h"

// Generated Headers
#include "CraftStorage.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UCraftStorage : public USaveGame
{

	GENERATED_BODY()

public:

	UPROPERTY(SaveGame)
	TMap<FTradeKey, FCraftData> CraftItems;

};

