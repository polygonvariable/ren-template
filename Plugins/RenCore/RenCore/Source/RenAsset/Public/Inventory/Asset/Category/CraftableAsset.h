// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenAsset/Public/Inventory/Asset/Category/PurchasableAsset.h"

// Generated Headers
#include "CraftableAsset.generated.h"

// Forward Declarations
class UInventoryAsset;


/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UCraftableAsset : public UPurchasableAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Craftable")
	TMap<UInventoryAsset*, int> CraftingCosts;

};
