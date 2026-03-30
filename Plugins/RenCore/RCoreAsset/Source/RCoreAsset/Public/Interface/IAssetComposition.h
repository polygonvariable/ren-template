// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "IAssetComposition.generated.h"

// Module Macros
#define REN_API RCOREASSET_API

// Forward Declarations
class UAssetCollection;



UINTERFACE(MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
class UAssetCompositionInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class REN_API IAssetCompositionInterface
{

	GENERATED_BODY()

public:

	virtual const UAssetCollection* GetBreakdownAssets(const FGuid& InId) const = 0;
	virtual const UAssetCollection* GetRebuildAssets(const FGuid& InId) const = 0;

};



// Module Macros
#undef REN_API

