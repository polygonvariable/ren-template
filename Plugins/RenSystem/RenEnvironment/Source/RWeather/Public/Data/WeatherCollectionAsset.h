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

    // ~ UPrimaryDataAsset
    virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
    // ~ End of UPrimaryDataAsset

#if WITH_EDITOR
    // ~ UPrimaryDataAsset
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
    // ~ End of UPrimaryDataAsset
#endif

protected:

    UPROPERTY(VisibleAnywhere)
    int TotalWeight = 0;

public:

    static FPrimaryAssetType GetPrimaryAssetType();

};

