// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Inventory/Type/InventoryAssetQuantity.h"

// Project Headers
#include "RenCoreInventory/Public/InventoryAsset.h"



TMap<FName, int> FInventoryAssetQuantity::GetItemIds() const
{
	TMap<FName, int> Result;
	for (auto& Pair : InventoryRecords)
	{
		if (Pair.Key)
		{
			Result.Add(Pair.Key->ItemId, Pair.Value);
		}
	}
	return Result;
}

