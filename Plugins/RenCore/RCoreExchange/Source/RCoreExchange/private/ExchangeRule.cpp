// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "ExchangeRule.h"

// Engine Headers

// Project Headers



int FExchangeRule::FindAny(const FPrimaryAssetId& AssetId, const FName& AssetType) const
{
	const int* AssetCount = RequiredAssets.Find(AssetId);
	if (AssetCount)
	{
		return *AssetCount;
	}

	const int* ArbitraryCount = RequiredArbitrary.Find(AssetType);
	if (ArbitraryCount)
	{
		return *ArbitraryCount;
	}

	return -1;
}

bool FExchangeRule::Contains(const FPrimaryAssetId& AssetId, const FName& AssetType) const
{
	return RequiredAssets.Contains(AssetId) && RequiredArbitrary.Contains(AssetType);
}

bool FExchangeRule::ContainsAny(const FPrimaryAssetId& AssetId, const FName& AssetType) const
{
	return RequiredAssets.Contains(AssetId) || RequiredArbitrary.Contains(AssetType);
}

