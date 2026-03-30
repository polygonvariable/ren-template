// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/InventoryItemRarity.h"
#include "Definition/InventoryItemType.h"

// Module Macros
#define REN_API RINVENTORY_API



/**
 *
 */
class REN_API FInventoryPrimaryAsset
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


// Module Macros
#undef REN_API

