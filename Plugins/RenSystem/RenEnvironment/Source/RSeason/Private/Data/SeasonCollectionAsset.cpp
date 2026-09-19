// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/SeasonCollectionAsset.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#include "Data/SeasonAsset.h"


const USeasonAsset* USeasonCollectionAsset::GetSeasonByDay(int InDay, int InYearLength, float& OutAlpha) const
{
    if (Seasons.Num() == 0 || InYearLength <= 0)
    {
        return nullptr;
    }

    const int Day = (InDay - 1 + InYearLength) % InYearLength;
    const int StartDay = (SeasonStartDay - 1 + InYearLength) % InYearLength;
    const int RelativeDay = (Day - StartDay + InYearLength) % InYearLength;

    int AccumulatedDays = 0;
    for (const FSeasonData& Season : Seasons)
    {
        const int SeasonStartRelativeDay = AccumulatedDays;
        AccumulatedDays += Season.Duration;

        if (RelativeDay < AccumulatedDays)
        {
            int SeasonDuration = Season.Duration;
            const USeasonAsset* SeasonAsset = Season.Asset;
            if (!IsValid(SeasonAsset) || !IsValid(SeasonAsset->TransitionCurve))
            {
                return nullptr;
            }

            const int LocalDay = RelativeDay - SeasonStartRelativeDay;
            const float Alpha = (SeasonDuration > 1) ? static_cast<float>(LocalDay) / static_cast<float>(SeasonDuration - 1) : 1.0f;
            const float Curve = SeasonAsset->TransitionCurve->GetFloatValue(Alpha);

            OutAlpha = FMath::Clamp(Curve, 0.0f, 1.0f);
            return SeasonAsset;
        }
    }

    return nullptr;
}

FPrimaryAssetId USeasonCollectionAsset::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(USeasonCollectionAsset::GetPrimaryAssetType(), GetFName());
}

FPrimaryAssetType USeasonCollectionAsset::GetPrimaryAssetType()
{
    return FPrimaryAssetType(TEXT("Season.Collection"));
}

#if WITH_EDITOR
EDataValidationResult USeasonCollectionAsset::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);

    if (Seasons.Num() == 0)
    {
        Context.AddError(FText::FromString("Season collection is empty"));
        return EDataValidationResult::Invalid;
    }
    else
    {
        int TotalYearDuration = 0;
        for (const FSeasonData& Item : Seasons)
        {
            if (!IsValid(Item.Asset))
            {
                Context.AddError(FText::FromString("Invalid weather asset in list"));
                return EDataValidationResult::Invalid;
            }
            TotalYearDuration += Item.Duration;
        }
    }

    return Result;
}
#endif

