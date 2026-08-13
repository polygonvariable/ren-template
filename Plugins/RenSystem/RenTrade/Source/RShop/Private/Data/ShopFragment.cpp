// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/ShopFragment.h"

// Project Headers
#include "Data/AssetCollection.h"
#include "Data/AssetGroup.h"


const UAssetCollection* UShopFragment::GetPurchaseCost() const
{
	if (!IsValid(PurchaseCost))
	{
		return nullptr;
	}
	return PurchaseCost->GetCollectionRule<UAssetCollection>();
}

const UAssetCollection* UShopFragment::GetPurchaseCost(const FInstancedStruct& Context) const
{
	if (!IsValid(PurchaseCost))
	{
		return nullptr;
	}
	return PurchaseCost->GetCollectionRule<UAssetCollection>(Context);
}

#if WITH_EDITORONLY_DATA
void UShopFragment::AppendAssetBundleData(FAssetBundleData& Data)
{
	Super::AppendAssetBundleData(Data);
}
#endif

