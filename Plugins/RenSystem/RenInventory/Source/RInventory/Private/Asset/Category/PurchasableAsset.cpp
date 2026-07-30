// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/Category/PurchasableAsset.h"

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"



const UAssetCollection* UPurchasableAsset::GetPurchaseCost() const
{
	if (!IsValid(PurchaseCost))
	{
		return nullptr;
	}
	return PurchaseCost->GetCollectionRule<UAssetCollection>();
}

const UAssetCollection* UPurchasableAsset::GetPurchaseCost(const FInstancedStruct& Context) const
{
	if (!IsValid(PurchaseCost))
	{
		return nullptr;
	}
	return PurchaseCost->GetCollectionRule<UAssetCollection>(Context);
}

