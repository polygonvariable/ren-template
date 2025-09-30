// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreInventory/Public/InventoryAsset.h"

// Generated Headers
#include "PurchasableAsset.generated.h"

// Forward Declarations



/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UPurchasableAsset : public UInventoryAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int PurchaseQuantity;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<UInventoryAsset*, int> PurchaseCosts;

};

