// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InventoryAsset.h"

// Engine Headers

// Project Headers


FPrimaryAssetId UInventoryAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("Inventory"), GetFName());
}

float UInventoryAsset::GetExchangedNumber(FInstancedStruct& Context) const
{
	return -1.0f;
}

FName UInventoryAsset::GetExchangedText(FInstancedStruct& Context) const
{
	return NAME_None;
}

