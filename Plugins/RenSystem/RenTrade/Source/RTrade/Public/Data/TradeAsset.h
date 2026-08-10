// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Asset/MetadataAsset.h"

// Generated Headers
#include "TradeAsset.generated.h"

// Module Macros
#define REN_API RTRADE_API

// Forward Declarations
class UAssetGroup;


/**
 *
 */
UCLASS(MinimalAPI)
class UTradeAsset : public UMetadataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> TradeGroup = nullptr;


	// ~ UObject
	REN_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UObject

};


// Module Macros
#undef REN_API

