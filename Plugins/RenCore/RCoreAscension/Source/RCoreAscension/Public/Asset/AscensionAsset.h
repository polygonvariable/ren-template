// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/MetadataAsset.h"

// Generated Headers
#include "AscensionAsset.generated.h"

// Module Macros
#define REN_API RCOREASCENSION_API

// Forward Declarations



/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class UExperiencePointAsset : public UMetadataAsset
{

	GENERATED_BODY()

public:

	REN_API virtual int GetPoints(int Quantity) const;

protected:

	UPROPERTY(EditDefaultsOnly)
	int Points = 0;

};



// Module Macros
#undef REN_API

