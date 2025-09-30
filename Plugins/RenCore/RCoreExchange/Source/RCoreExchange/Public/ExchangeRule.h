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

	int FindAny(const FPrimaryAssetId& AssetId, const FName& AssetType) const
	{
		const int* AssetCount = RequiredAssets.Find(AssetId);
		if (AssetCount)
		{
			return *AssetCount;
		}

		const int* ArbitraryCount = RequiredArbitrary.Find(AssetType);
		if (ArbitraryCount)
		{
			return *ArbitraryCount;
		}

		return -1;
	}

	bool Contains(const FPrimaryAssetId& AssetId, const FName& AssetType) const
	{
		return RequiredAssets.Contains(AssetId) && RequiredArbitrary.Contains(AssetType);
	}

	bool ContainsAny(const FPrimaryAssetId& AssetId, const FName& AssetType) const
	{
		return RequiredAssets.Contains(AssetId) || RequiredArbitrary.Contains(AssetType);
	}

};

