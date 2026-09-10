// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherEdAction.h"

// Project Headers
#include "Data/WeatherAsset.h"
#include "Data/WeatherCollectionAsset.h"
#include "Data/WeatherWorldConfig.h"


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


FText FWeatherWorldConfigActions::GetName() const
{
    return FText::FromString(TEXT("Weather World Config"));
}

FColor FWeatherWorldConfigActions::GetTypeColor() const
{
    return FColor::FromHex(TEXT("#317C7C"));
}

UClass* FWeatherWorldConfigActions::GetSupportedClass() const
{
    return UWeatherWorldConfig::StaticClass();
}

uint32 FWeatherWorldConfigActions::GetCategories()
{
    return EAssetTypeCategories::Misc;
}

