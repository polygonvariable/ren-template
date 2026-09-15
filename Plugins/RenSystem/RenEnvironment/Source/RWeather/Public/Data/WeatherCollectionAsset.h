// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Project Headers
#include "Core/Type/WeatherWeight.h"

// Generated Headers
#include "WeatherCollectionAsset.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class UWeatherAsset;


/**
 *
 */
UCLASS(MinimalAPI)
class UWeatherCollectionAsset : public UPrimaryDataAsset
{

    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere)
    int Priority = 20;

    UPROPERTY(EditAnywhere)
    TArray<FWeightedWeatherId> Weathers;


    UWeatherAsset* GetRandomWeather() const;

#if WITH_EDITOR
    // ~ UPrimaryDataAsset
    virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
    // ~ End of UPrimaryDataAsset
#endif

    // ~ UPrimaryDataAsset
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
    // ~ End of UPrimaryDataAsset

    static FPrimaryAssetType GetPrimaryAssetType();

protected:

    UPROPERTY(VisibleAnywhere)
    int TotalWeight = 0;

};

