// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/InventoryItemRarity.h"
#include "Core/Type/InventoryItemType.h"

// Module Macros
#define REN_API RINVENTORY_API


/**
 *
 */
namespace FInventoryPrimaryAsset
{

	REN_API FPrimaryAssetType GetAssetType();
	REN_API FPrimaryAssetId GetPrimaryAssetId(const FName& AssetName);

	REN_API bool IsValid(const FPrimaryAssetId& AssetId);

	REN_API bool GetType(const FAssetData& AssetData, FName& Type);
	REN_API bool GetType(const FAssetData& AssetData, EInventoryItemType& Type);

	REN_API bool GetRarity(const FAssetData& AssetData, FName& Rarity);
	REN_API bool GetRarity(const FAssetData& AssetData, EInventoryItemRarity& Rarity);

	REN_API bool GetDisplayName(const FAssetData& AssetData, FText& DisplayName);
	REN_API bool GetStackable(const FAssetData& AssetData, bool& bStackable);
	REN_API bool GetPersistWhenEmpty(const FAssetData& AssetData, bool& bPersistWhenEmpty);

};


// Module Macros
#undef REN_API

