// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AssetDetail_Trade.h"
#include "Widget/AssetEntry.h"

// Generated Headers
#include "TradeEntry.generated.h"

// Module Macros
#define REN_API RTRADEUI_API

// Forward Declarations



/**
 *
 */
UCLASS(MinimalAPI)
class UTradeEntry : public UAssetEntry
{

	GENERATED_BODY()

public:

	FAssetDetail_Trade TradeDetail;

	// ~ UAssetEntry
	REN_API virtual void ResetData() override
	{
		TradeDetail.Reset();
		Super::ResetData();
	}
	// ~ End of UAssetEntry

};



// Module Macros
#undef REN_API

