// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/InventoryAsset.h"
#include "Interface/IShopProvider.h"

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
class UPurchasableAsset : public UInventoryAsset, public IShopProvider
{

	GENERATED_BODY()

public:

	// ~ IShopProvider
	REN_API virtual const UAssetCollection* GetPurchaseCost() const override;
	REN_API virtual const UAssetCollection* GetPurchaseCost(const FInstancedStruct& Context) const override;
	// ~ End of IShopProvider

protected:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> PurchaseCost = nullptr;

};



// Module Macros
#undef REN_API

