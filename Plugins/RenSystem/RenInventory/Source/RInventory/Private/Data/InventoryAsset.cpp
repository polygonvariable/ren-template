// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/InventoryAsset.h"


FPrimaryAssetType UInventoryAsset::GetPrimaryAssetType()
{
	return TEXT("Asset.Inventory");
}

FPrimaryAssetId UInventoryAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UInventoryAsset::GetPrimaryAssetType(), GetFName());
}

