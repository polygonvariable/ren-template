// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/Category/PurchasableAsset.h"
#include "Interface/ICraftProvider.h"

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
class UCraftableAsset : public UPurchasableAsset, public ICraftProvider
{

	GENERATED_BODY()

public:

	// ~ ICraftProvider
	REN_API virtual FTimespan GetCraftingTime() const override;
	REN_API virtual const UAssetCollection* GetCraftingMaterial() const override;
	REN_API virtual const UAssetCollection* GetCraftingMaterial(const FInstancedStruct& Context) const override;
	// ~ End of ICraftProvider

protected:

	UPROPERTY(EditDefaultsOnly)
	FTimespan CraftingTime;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> CraftingItems = nullptr;

};



// Module Macros
#undef REN_API

