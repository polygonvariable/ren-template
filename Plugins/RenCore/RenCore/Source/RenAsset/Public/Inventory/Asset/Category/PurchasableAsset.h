// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenCoreInventory/Public/InventoryAsset.h"

// Generated Headers
#include "PurchasableAsset.generated.h"


/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UPurchasableAsset : public UInventoryAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Purchasable")
	int PurchaseQuantity;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Purchasable")
	TMap<UInventoryAsset*, int> PurchaseCosts;

};