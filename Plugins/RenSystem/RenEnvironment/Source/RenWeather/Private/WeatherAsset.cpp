// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherAsset.h"

// Engine Headers

// Project Header



FPrimaryAssetType UWeatherAsset::GetPrimaryAssetType()
{
    return FPrimaryAssetType(TEXT("Environment.Weather"));
}

FPrimaryAssetId UWeatherAsset::MakePrimaryAssetId(const FName& AssetName)
{
    return FPrimaryAssetId(GetPrimaryAssetType(), AssetName);
}

bool UWeatherAsset::IsValid(const FPrimaryAssetId& AssetId)
{
    return AssetId.PrimaryAssetType == GetPrimaryAssetType();
}

FPrimaryAssetId UWeatherAsset::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(GetPrimaryAssetType(), GetFName());
}

