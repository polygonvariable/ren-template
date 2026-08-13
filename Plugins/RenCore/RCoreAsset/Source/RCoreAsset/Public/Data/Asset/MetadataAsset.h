// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Data/CoreDataAsset.h"

// Generated Headers
#include "MetadataAsset.generated.h"

// Module Macros
#define REN_API RCOREASSET_API


/**
 * 
 */
UCLASS(Abstract, MinimalAPI)
class UMetadataAsset : public UCoreDataAsset
{

	GENERATED_BODY()

public:

	// ~ UPrimaryDataAsset
	REN_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

};


// Module Macros
#undef REN_API

