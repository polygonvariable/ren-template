// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/CraftFragment.h"

// Project Headers
#include "Data/AssetCollection.h"
#include "Data/AssetGroup.h"


FTimespan UCraftFragment::GetCraftingTime() const
{
	return CraftingTime;
}

const UAssetCollection* UCraftFragment::GetCraftingMaterial() const
{
	if (!IsValid(CraftingItems))
	{
		return nullptr;
	}
	return CraftingItems->GetCollectionRule<UAssetCollection>();
}

const UAssetCollection* UCraftFragment::GetCraftingMaterial(const FInstancedStruct& Context) const
{
	if (!IsValid(CraftingItems))
	{
		return nullptr;
	}
	return CraftingItems->GetCollectionRule<UAssetCollection>(Context);
}

#if WITH_EDITORONLY_DATA
void UCraftFragment::AppendAssetBundleData(FAssetBundleData& Data)
{
	Super::AppendAssetBundleData(Data);
}
#endif

