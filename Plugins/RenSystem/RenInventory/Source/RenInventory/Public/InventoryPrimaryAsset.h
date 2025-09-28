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

	static bool GetItemType(const FAssetData& AssetData, FName& ItemType);
	static bool GetItemType(const FAssetData& AssetData, EInventoryItemType& ItemType);

	static bool GetItemRarity(const FAssetData& AssetData, FName& ItemRarity);
	static bool GetItemRarity(const FAssetData& AssetData, EInventoryItemRarity& ItemRarity);

	static bool GetItemName(const FAssetData& AssetData, FText& ItemName);
	static bool GetItemIsStackable(const FAssetData& AssetData, bool& bIsStackable);
	static bool GetItemAllowEmptyData(const FAssetData& AssetData, bool& bOutAllowEmptyData);

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

