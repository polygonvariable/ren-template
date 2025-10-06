// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "ExchangeQuota.generated.h"



/**
 *
 */
USTRUCT(BlueprintType)
struct RCOREEXCHANGE_API FExchangeQuota
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Limit = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUnlimited = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimespan Cooldown = FTimespan::Zero();

};



/**
 *
 */
USTRUCT(BlueprintType)
struct RCOREEXCHANGE_API FExchangeRecord
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FExchangeQuota Quota;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Quantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime LastExchangeTime = FDateTime::Now();

};



/**
 *
 */
USTRUCT(BlueprintType)
struct RCOREEXCHANGE_API FExchangeCatalog : public FTableRowBase
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FPrimaryAssetId, FExchangeQuota> Stock;

};

