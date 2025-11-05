// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Project Headers
#include "RCoreMaterial/Public/MaterialSurfaceProperty.h"
#include "RenWeather/Public/WeatherSurfaceEffect.h"

// Generated Headers
#include "WeatherAsset.generated.h"

// Forward Declarations
class AWeatherEffectActor;



/**
 *
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

    UPROPERTY(EditDefaultsOnly, Meta = (AllowedTypes = "Environment.Profile"), Category = "Environment Profile")
    TArray<FPrimaryAssetId> EnvironmentProfiles;

    UPROPERTY(EditDefaultsOnly, Category = "Environment Profile")
    int ProfilePriority = 0;


    static FPrimaryAssetType GetPrimaryAssetType();
    static FPrimaryAssetId MakePrimaryAssetId(const FName& AssetName);

    static bool IsValid(const FPrimaryAssetId& AssetId);

public:

    // ~ UPrimaryDataAsset
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
    // ~ End of UPrimaryDataAsset

};

