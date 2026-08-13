// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Data/Asset/InstanceableDataAsset.h"

// Generated Headers
#include "RewardAsset.generated.h"

// Module Macros
#define REN_API RREWARD_API

// Forward Declarations
class UAssetCollection;


/**
 *
 */
UCLASS(MinimalAPI)
class URewardAsset : public UInstanceableDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetCollection> AssetCollection = nullptr;


	// ~ UPrimaryDataAsset
	REN_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

public:

	static REN_API FPrimaryAssetType GetPrimaryAssetType();

};


// Module Macros
#undef REN_API

