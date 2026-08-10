// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/InventoryAsset.h"

// Generated Headers
#include "PurchasableAsset.generated.h"

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
class UPurchasableAsset : public UInventoryAsset
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> PurchaseCost = nullptr;

};



// Module Macros
#undef REN_API

