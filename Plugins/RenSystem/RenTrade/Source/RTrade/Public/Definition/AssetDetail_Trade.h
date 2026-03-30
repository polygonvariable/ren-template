// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/AssetDetail.h"

// Generated Headers
#include "AssetDetail_Trade.generated.h"

// Forward Declarations
class UCoreDataAsset;



/**
 *
 *
 */
USTRUCT(BlueprintType)
struct FAssetDetail_Trade : public FAssetDetail
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

