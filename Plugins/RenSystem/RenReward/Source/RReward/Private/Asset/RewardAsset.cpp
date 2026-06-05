// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/RewardAsset.h"


FPrimaryAssetType URewardAsset::GetPrimaryAssetType()
{
	return TEXT("Asset.Reward");
}

FPrimaryAssetId URewardAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(URewardAsset::GetPrimaryAssetType(), GetFName());
}

