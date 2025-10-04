// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreInventory/Public/InventoryItemRarity.h"
#include "RCoreInventory/Public/InventoryItemType.h"

// Generated Headers

// Forward Declarations



/**
 *
 */
class RENINVENTORY_API InventoryPrimaryAsset
{

public:

	static FPrimaryAssetType GetAssetType();
	static FPrimaryAssetId GetPrimaryAssetId(const FName& AssetName);

	static bool IsValid(const FPrimaryAssetId& AssetId);

	static bool GetType(const FAssetData& AssetData, FName& Type);
	static bool GetType(const FAssetData& AssetData, EInventoryItemType& Type);

	static bool GetRarity(const FAssetData& AssetData, FName& Rarity);
	static bool GetRarity(const FAssetData& AssetData, EInventoryItemRarity& Rarity);

	static bool GetDisplayName(const FAssetData& AssetData, FText& DisplayName);
	static bool GetStackable(const FAssetData& AssetData, bool& bStackable);
	static bool GetPersistWhenEmpty(const FAssetData& AssetData, bool& bPersistWhenEmpty);

};


/**
 *
 */
class RENINVENTORY_API InventoryFilterProperty
{

public:

	static const FName AssetId;
	static const FName AssetType;
	static const FName AssetRarity;

	static const FName ItemId;
	static const FName ItemQuantity;
	static const FName ItemExperience;
	static const FName ItemLevel;
	static const FName ItemRank;

};

