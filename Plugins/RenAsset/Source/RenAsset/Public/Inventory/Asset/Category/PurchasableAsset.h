// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "Inventory/InventoryAsset.h"

// Generated Headers
#include "PurchasableAsset.generated.h"


/**
 * 
 */
UCLASS(Abstract)
class RENASSET_API UPurchasableAsset : public UInventoryAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Purchasable")
	int32 PurchaseQuantity;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Purchasable")
	TMap<UInventoryAsset*, int32> PurchaseCosts;

};