// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "ICraftProvider.generated.h"

// Module Macros
#define REN_API RCORECRAFT_API

// Forward Declarations
class UAssetCollection;

struct FInstancedStruct;



UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCraftProvider : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class REN_API ICraftProvider
{

	GENERATED_BODY()

public:

	virtual FTimespan GetCraftingTime() const = 0;
	virtual const UAssetCollection* GetCraftingMaterial() const = 0;
	virtual const UAssetCollection* GetCraftingMaterial(const FInstancedStruct& Context) const = 0;

};



// Module Macros
#undef REN_API

