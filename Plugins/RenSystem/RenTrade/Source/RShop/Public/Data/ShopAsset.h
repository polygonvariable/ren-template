// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Data/TradeAsset.h"

// Generated Headers
#include "ShopAsset.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UShopAsset : public UTradeAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
    FTimespan RefreshTime;

};

