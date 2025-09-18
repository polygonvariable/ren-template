// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreInventory/Public/InventoryItemType.h"
#include "RCoreInventory/Public/InventoryItemRarity.h"

// Generated Headers

// Forward Declarations



class InventoryPrimaryAsset
{

public:

	static FPrimaryAssetType GetAssetType();

	static FPrimaryAssetId GetPrimaryAssetId(const FName& ItemId);

	static bool IsValid(const FPrimaryAssetId& AssetId);

	static bool GetItemType(const FAssetData& AssetData, EInventoryItemType& ItemType);

	static bool GetItemRarity(const FAssetData& AssetData, EInventoryItemRarity& ItemRarity);

	static bool GetItemName(const FAssetData& AssetData, FText& ItemName);

	static bool GetItemIsStackable(const FAssetData& AssetData, bool& bIsStackable);

	static bool GetItemAllowEmptyData(const FAssetData& AssetData, bool& bOutAllowEmptyData);

};

