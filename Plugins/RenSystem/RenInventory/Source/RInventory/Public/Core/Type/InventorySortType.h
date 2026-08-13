// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/QueryType.h"

// Generated Headers
#include "InventorySortType.generated.h"

// Forward Declarations
struct FInventoryInstance;


/**
 *
 * 
 */
UENUM(BlueprintType)
enum class EInventorySortType : uint8
{
	None UMETA(DisplayName = "None"),
	Alphabetical UMETA(DisplayName = "Alphabetical"),
	Quantity UMETA(DisplayName = "Quantity"),
	Rank UMETA(DisplayName = "Rank"),
	Level UMETA(DisplayName = "Level"),
};


/**
 *
 */
struct FInventorySortEntry
{

public:

	FInventorySortEntry(FPrimaryAssetId InAssetId, FText InDisplayName, int InQuantity) : AssetId(InAssetId), DisplayName(InDisplayName), Quantity(InQuantity) {}
	FInventorySortEntry(FPrimaryAssetId InAssetId, FText InDisplayName, int InQuantity, const FInventoryInstance* InInstance, bool bInIsLinked) : AssetId(InAssetId), DisplayName(InDisplayName), Quantity(InQuantity), Instance(InInstance), bIsLinked(bInIsLinked) {}

	FPrimaryAssetId AssetId;
	FText DisplayName;
	int Quantity = 0;
	const FInventoryInstance* Instance = nullptr;
	bool bIsLinked = false;

};

