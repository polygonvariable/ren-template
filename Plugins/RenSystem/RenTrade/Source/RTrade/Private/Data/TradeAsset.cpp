// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/TradeAsset.h"


FPrimaryAssetId UTradeAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("Asset.Trade"), GetFName());
}

