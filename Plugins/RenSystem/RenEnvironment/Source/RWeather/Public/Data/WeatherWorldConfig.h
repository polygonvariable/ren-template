// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "WorldConfigAsset.h"

// Generated Headers
#include "WeatherWorldConfig.generated.h"

class UMaterialParameterCollection;
class UWeatherController;
class AWeatherEffectManager;


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

    UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (AllowedTypes = "Weather"))
    FPrimaryAssetId DefaultWeather;

    UPROPERTY(EditDefaultsOnly, Category = "Default")
    int DefaultPriority = 0;

    UPROPERTY(EditDefaultsOnly, Category = "Material")
    TObjectPtr<UMaterialParameterCollection> WeatherMPC;

    UPROPERTY(EditDefaultsOnly, Category = "Controller")
    TSubclassOf<UWeatherController> WeatherController;

    UPROPERTY(EditDefaultsOnly, Category = "Controller")
    TSubclassOf<AWeatherEffectManager> EffectManager;

    UPROPERTY(EditDefaultsOnly, Category = "Weather")
    float RefreshDuration = 5.0f;


#if WITH_EDITOR
    // ~ UPrimaryDataAsset
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
    // ~ End of UPrimaryDataAsset
#endif

};

