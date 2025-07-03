// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "Inventory/Asset/Category/CraftableAsset.h"
#include "Inventory/Type/InventoryAssetQuantity.h"
#include "RenCore/Public/Inventory/InventoryItemRarity.h"
#include "RenCore/Public/Inventory/InventoryItemType.h"

// Generated Headers
#include "EnhanceableAsset.generated.h"

// Forward Declarations
class UEnhanceAsset;


/**
 * 
 */
UCLASS(Abstract, DisplayName = "Enhanceable Asset")
class RENASSET_API UEnhanceableAsset : public UCraftableAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	int32 EnhanceXpInterval = 5000;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	int32 EnhanceLevelInterval = 10;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	int32 EnhanceLevelMax = 100;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	TSet<UEnhanceAsset*> EnhanceCosts;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	TMap<TEnumAsByte<EInventoryItemType>, int> EnhanceExternalCosts;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	TMap<int, FInventoryAssetQuantity> EnhanceRankings;

};