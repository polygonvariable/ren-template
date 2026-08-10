// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/Category/PurchasableAsset.h"

// Generated Headers
#include "CraftableAsset.generated.h"

// Module Macros
#define REN_API RINVENTORY_API

// Forward Declarations
class UAssetGroup;
class UAssetCollection;
struct FInstancedStruct;



/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UCraftableAsset : public UPurchasableAsset
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly)
	FTimespan CraftingTime;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> CraftingItems = nullptr;

};



// Module Macros
#undef REN_API

