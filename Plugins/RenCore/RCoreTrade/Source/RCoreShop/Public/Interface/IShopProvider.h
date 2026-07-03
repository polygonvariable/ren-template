// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "IShopProvider.generated.h"

// Module Macros
#define REN_API RCORESHOP_API

// Forward Declarations
class UAssetCollection;

struct FInstancedStruct;



UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UShopProvider : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class REN_API IShopProvider
{

	GENERATED_BODY()

public:

	virtual const UAssetCollection* GetPurchaseCost() const = 0;
	virtual const UAssetCollection* GetPurchaseCost(const FInstancedStruct& Context) const = 0;

};



// Module Macros
#undef REN_API

