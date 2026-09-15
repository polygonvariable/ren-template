// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/WeatherCollectionAsset.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#include "UObject/ObjectSaveContext.h"
#endif

// Project Headers
#include "Data/WeatherAsset.h"


UWeatherAsset* UWeatherCollectionAsset::GetRandomWeather() const
{
    float Random = FMath::FRandRange(0.0f, TotalWeight);
    float Cumulative = 0.0f;

    for (const FWeightedWeatherId& Weather : Weathers)
    {
        Cumulative += Weather.Weight;
        if (Random <= Cumulative)
        {
            return Weather.Asset;
        }
    }
    return nullptr;
}

#if WITH_EDITOR
void UWeatherCollectionAsset::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
    Super::PreSave(ObjectSaveContext);

    TotalWeight = 0;
    for (const FWeightedWeatherId& Item : Weathers)
    {
        TotalWeight += Item.Weight;
    }
}
EDataValidationResult UWeatherCollectionAsset::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);

    if (Weathers.Num() == 0)
    {
        Context.AddError(FText::FromString("Weather collection is empty"));
        Result = EDataValidationResult::Invalid;
    }
    else
    {
        for (const FWeightedWeatherId& Item : Weathers)
        {
            if (!IsValid(Item.Asset))
            {
                Context.AddError(FText::FromString("Invalid weather asset in list"));
                Result = EDataValidationResult::Invalid;
                break;
            }
        }
    }

    return Result;
}
#endif

FPrimaryAssetId UWeatherCollectionAsset::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(GetPrimaryAssetType(), GetFName());
}

FPrimaryAssetType UWeatherCollectionAsset::GetPrimaryAssetType()
{
    return FPrimaryAssetType(TEXT("Weather.Collection"));
}

