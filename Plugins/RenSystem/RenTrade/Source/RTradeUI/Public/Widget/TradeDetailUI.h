// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDetailUI.h"

// Generated Headers
#include "TradeDetailUI.generated.h"

// Module Macros
#define REN_API RTRADEUI_API

// Forward Declarations
class UAssetEntry;
class UCoreDataAsset;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UTradeDetailUI : public UAssetDetailUI
{

	GENERATED_BODY()

public:

	FPrimaryAssetId TradeAssetId;
	FGuid TradeCollectionId;

	// ~ UAssetDetailUI
	REN_API virtual void ResetDetail() override;
	// ~ End of UAssetDetailUI

protected:

	// ~ UAssetDetailUI
	REN_API virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	// ~ End of UAssetDetailUI

};



// Module Macros
#undef REN_API

