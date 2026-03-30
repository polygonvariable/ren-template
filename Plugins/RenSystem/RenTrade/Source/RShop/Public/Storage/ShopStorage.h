// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/Runtime/ShopData.h"
#include "Definition/Runtime/TradeKey.h"
#include "SaveGame/Storage.h"

// Generated Headers
#include "ShopStorage.generated.h"

// Module Macros
#define REN_API RSHOP_API

// Forward Declarations



/**
 *
 */
UCLASS(MinimalAPI)
class UShopStorage : public UStorage
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE(FOnShopUpdated);
	FOnShopUpdated OnShopUpdated;


	REN_API const FShopData* GetItem(const FTradeKey& TradeKey) const;
	REN_API bool AddItem(const FTradeKey& TradeKey);
	REN_API void ResetItems();

protected:

	UPROPERTY(SaveGame)
	TMap<FTradeKey, FShopData> ShopItems;

};



// Module Macros
#undef REN_API

