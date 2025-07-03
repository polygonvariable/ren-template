// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Parent Header
#include "Inventory/Type/InventoryAssetQuantity.h"

// Project Headers
#include "Inventory/InventoryAsset.h"

TMap<FName, int> FInventoryAssetQuantity::ConvertToIds()
{
	TMap<FName, int> Ids;
	for (auto& Pair : InventoryRecords)
	{
		Ids.Add(Pair.Key->AssetId, Pair.Value);
	}
	return Ids;
}

