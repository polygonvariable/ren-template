// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "WeatherSurfaceEffect.generated.h"

// Forward Declarations



/**
 *
 * 
 */
USTRUCT(BlueprintType)
struct FWeatherSurfaceEffect
{

    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "0", UIMax = "1", ClampMin = "0", ClampMax = "1"))
    float WindIntensity = 0.0f;

    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "0", UIMax = "1", ClampMin = "0", ClampMax = "1"))
    float RainIntensity = 0.0f;

    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "0", UIMax = "1", ClampMin = "0", ClampMax = "1"))
    float SnowIntensity = 0.0f;

    void Reset();
    void Clamp();

    void GetParameters(UMaterialParameterCollectionInstance* Instance, FName WindName, FName RainName, FName SnowName);
    void SetParameters(UMaterialParameterCollectionInstance* Instance, FName WindName, FName RainName, FName SnowName);

    static FWeatherSurfaceEffect Lerp(const FWeatherSurfaceEffect& A, const FWeatherSurfaceEffect& B, float Alpha);

};

