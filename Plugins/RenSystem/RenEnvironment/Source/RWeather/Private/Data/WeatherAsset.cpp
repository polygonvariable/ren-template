// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/WeatherAsset.h"


FPrimaryAssetId UWeatherAsset::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(GetPrimaryAssetType(), GetFName());
}

FPrimaryAssetType UWeatherAsset::GetPrimaryAssetType()
{
    return FPrimaryAssetType(TEXT("Environment.Weather"));
}

