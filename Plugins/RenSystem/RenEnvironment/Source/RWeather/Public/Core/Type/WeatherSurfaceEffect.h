// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "WeatherSurfaceEffect.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FWeatherSurfaceEffect
{

    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, meta = (UIMin = "0", UIMax = "1", ClampMin = "0", ClampMax = "1"))
    float RainIntensity = 0.0f;

    UPROPERTY(EditDefaultsOnly, meta = (UIMin = "0", UIMax = "1", ClampMin = "0", ClampMax = "1"))
    float SnowIntensity = 0.0f;

    void Reset();
    void Clamp();

    void GetParameters(UMaterialParameterCollectionInstance* MPCInstance);
    void SetParameters(UMaterialParameterCollectionInstance* MPCInstance);

    static FWeatherSurfaceEffect Lerp(const FWeatherSurfaceEffect& A, const FWeatherSurfaceEffect& B, float Alpha);

};

