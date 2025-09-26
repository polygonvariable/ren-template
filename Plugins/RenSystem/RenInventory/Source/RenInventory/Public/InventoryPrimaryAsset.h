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
class InventoryPrimaryAsset
{

public:

	static FPrimaryAssetType GetAssetType();
	RENINVENTORY_API static FPrimaryAssetId GetPrimaryAssetId(const FName& AssetName);

	static bool IsValid(const FPrimaryAssetId& AssetId);

	static bool GetItemType(const FAssetData& AssetData, FName& ItemType);
	static bool GetItemType(const FAssetData& AssetData, EInventoryItemType& ItemType);

	static bool GetItemRarity(const FAssetData& AssetData, FName& ItemRarity);
	static bool GetItemRarity(const FAssetData& AssetData, EInventoryItemRarity& ItemRarity);

	static bool GetItemName(const FAssetData& AssetData, FText& ItemName);
	static bool GetItemIsStackable(const FAssetData& AssetData, bool& bIsStackable);
	static bool GetItemAllowEmptyData(const FAssetData& AssetData, bool& bOutAllowEmptyData);

};

