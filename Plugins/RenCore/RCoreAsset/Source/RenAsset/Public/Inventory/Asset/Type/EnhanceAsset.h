// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenAsset/Public/Inventory/Asset/Category/NonEnhanceableAsset.h"

// Generated Headers
#include "EnhanceAsset.generated.h"



/**
 * 
 */
UCLASS(MinimalAPI)
class UEnhanceAsset : public UNonEnhanceableAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhancement")
	int ExchangePoints = 500;

public:

	// ~ IExchangeInterface
	RENASSET_API virtual float GetExchangedNumber(FInstancedStruct& Context) const override
	{
		return ExchangePoints;
	}
	// ~ End of IExchangeInterface

};

