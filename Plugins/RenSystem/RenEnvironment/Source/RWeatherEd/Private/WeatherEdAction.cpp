// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherEdAction.h"

// Project Headers
#include "WeatherAsset.h"


FText FWeatherAssetActions::GetName() const
{
    return FText::FromString(TEXT("Weather Asset"));
}

FColor FWeatherAssetActions::GetTypeColor() const
{
    return FColor::FromHex(TEXT("#317C7C"));
}

UClass* FWeatherAssetActions::GetSupportedClass() const
{
    return UWeatherAsset::StaticClass();
}

uint32 FWeatherAssetActions::GetCategories()
{
    return EAssetTypeCategories::Misc;
}

