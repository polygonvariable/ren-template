// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "InventoryAssetQuantity.generated.h"

// Forward Declarations
class UInventoryAsset;



/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryAssetQuantity
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UInventoryAsset*, int> InventoryRecords;

	RENASSET_API TMap<FName, int> GetItemIds() const;

};

