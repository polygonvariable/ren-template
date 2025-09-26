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
struct FExchangeRule : public FTableRowBase
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FPrimaryAssetId, int> RequiredAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> RequiredArbitrary;

	bool Contains(const FPrimaryAssetId& AssetId, const FName& AssetType)
	{
		return RequiredAssets.Contains(AssetId) && RequiredArbitrary.Contains(AssetType);
	}

	bool ContainsAny(const FPrimaryAssetId& AssetId, const FName& AssetType)
	{
		return RequiredAssets.Contains(AssetId) || RequiredArbitrary.Contains(AssetType);
	}

};

