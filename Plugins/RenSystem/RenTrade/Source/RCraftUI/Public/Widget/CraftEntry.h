// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/Runtime/CraftData.h"
#include "Widget/TradeEntry.h"

// Generated Headers
#include "CraftEntry.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UCraftEntry : public UTradeEntry
{

	GENERATED_BODY()

public:

	FCraftData CraftData;

	// ~ UAssetEntry
	virtual void ResetData() override
	{
		CraftData.Reset();
		Super::ResetData();
	}
	// ~ End of UAssetEntry

};

