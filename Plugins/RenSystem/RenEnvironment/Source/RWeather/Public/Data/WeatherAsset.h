// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Project Headers
#include "Core/Type/WeatherSurfaceEffect.h"
#include "MaterialSurfaceProperty.h"

// Generated Headers
#include "WeatherAsset.generated.h"

// Forward Declarations
class AWeatherEffectActor;
class UEnvironmentProfileAsset;


/**
 *
 */
UCLASS(MinimalAPI)
class UWeatherAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, Category = "Weather Details")
    FName WeatherName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Category = "Weather Effect")
    FMaterialSurfaceProperty SurfaceProperty;

    UPROPERTY(EditDefaultsOnly, Category = "Weather Effect")
    FWeatherSurfaceEffect SurfaceEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Weather Effect")
    TArray<TSoftClassPtr<AWeatherEffectActor>> EffectClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Environment")
    TArray<TObjectPtr<UEnvironmentProfileAsset>> EnvironmentProfiles;

    UPROPERTY(EditDefaultsOnly, Category = "Environment")
    int ProfilePriority = 0;

    UPROPERTY(EditDefaultsOnly, Category = "Transition", meta = (UIMin = "0.05", ClampMin = "0.05", UIMax = "30", ClampMax = "30"))
    float TransitionRate = 0.5f;

    UPROPERTY(EditDefaultsOnly, Category = "Transition", meta = (UIMin = "0.05", ClampMin = "0.05", UIMax = "30", ClampMax = "30"))
    float TransitionDuration = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Transition")
    TObjectPtr<UCurveFloat> TransitionCurve;


    // ~ UPrimaryDataAsset
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
    // ~ End of UPrimaryDataAsset

    static FPrimaryAssetType GetPrimaryAssetType();

};

