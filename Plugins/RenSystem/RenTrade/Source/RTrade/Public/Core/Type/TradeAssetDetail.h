// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/AssetDetail.h"

// Generated Headers
#include "TradeAssetDetail.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FTradeAssetDetail : public FAssetDetail
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	int Quota = 0;

	void Reset()
	{
		Quota = 0;
		FAssetDetail::Reset();
	}

};

