// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Data/EnvironmentAsset.h"
#include "WorldConfigAsset.h"

// Generated Headers
#include "WeatherWorldConfig.generated.h"

// Forward Declarations
class UMaterialParameterCollection;
class UWeatherController;
class AWeatherEffectManagerActor;


/**
 *
 */
UCLASS(MinimalAPI)
class UWeatherWorldConfig : public UWorldConfigAsset
{

    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, Category = "Weather")
    bool bEnabled = true;

    UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowedTypes = "Environment.Weather"))
    FPrimaryAssetId DefaultWeather;

    UPROPERTY(EditDefaultsOnly, Category = "Default")
    int DefaultPriority = 0;

    UPROPERTY(EditDefaultsOnly, Category = "Material")
    TObjectPtr<UMaterialParameterCollection> WeatherMPC;

    UPROPERTY(EditDefaultsOnly, Category = "Controller")
    TSubclassOf<UWeatherController> WeatherController;

    UPROPERTY(EditDefaultsOnly, Category = "Controller")
    TSubclassOf<AWeatherEffectManagerActor> EffectManager;

    UPROPERTY(EditDefaultsOnly, Category = "Weather")
    float RefreshDuration = 5.0f;

    // ~ UPrimaryDataAsset
#if WITH_EDITORONLY_DATA
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
    // ~ End of UPrimaryDataAsset

};

