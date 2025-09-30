// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenInventory/Public/Asset/Category/PurchasableAsset.h"

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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<UInventoryAsset*, int> CraftingCosts;

};

