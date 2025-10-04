// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "ExchangeRule.generated.h"



/**
 * 
 */
USTRUCT(BlueprintType)
struct RCOREEXCHANGE_API FExchangeRule : public FTableRowBase
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FPrimaryAssetId, int> RequiredAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int> RequiredArbitrary;

	int FindAny(const FPrimaryAssetId& AssetId, const FName& AssetType) const;
	bool Contains(const FPrimaryAssetId& AssetId, const FName& AssetType) const;
	bool ContainsAny(const FPrimaryAssetId& AssetId, const FName& AssetType) const;

};



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

