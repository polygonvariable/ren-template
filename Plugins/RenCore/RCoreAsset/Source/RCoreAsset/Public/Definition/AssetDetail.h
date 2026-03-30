// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "AssetDetail.generated.h"

// Forward Declarations
class UCoreDataAsset;


/**
 *
 */
USTRUCT(BlueprintType)
struct FAssetDetail
{

	GENERATED_BODY()

public:

	FAssetDetail() {}
	FAssetDetail(int InQuantity) : Quantity(InQuantity) {}

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UCoreDataAsset> DataAsset;

#endif

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Quantity = 0;

	void Reset()
	{
		Quantity = 0;

#if WITH_EDITOR
		DataAsset.Reset();
#endif
	}

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FAssetDetail_Unique : public FAssetDetail
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FGuid ItemId;

	void Reset()
	{
		ItemId.Invalidate();
		FAssetDetail::Reset();
	}

};

