// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/SaveGame.h"

// Project Headers
#include "Definition/Runtime/ShopData.h"
#include "Definition/Runtime/TradeKey.h"

// Generated Headers
#include "ShopStorage.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UShopStorage : public USaveGame
{

	GENERATED_BODY()

public:

	UPROPERTY(SaveGame)
	TMap<FTradeKey, FShopData> ShopItems;

};

