// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EnvironmentProfileType.h"
#include "Data/Asset/MetadataAsset.h"

// Generated Headers
#include "EnvironmentProfileAsset.generated.h"


/**
 * 
 */
UCLASS(Abstract, MinimalAPI)
class UEnvironmentProfileAsset : public UMetadataAsset
{

    GENERATED_BODY()

public:

    UPROPERTY(VisibleAnywhere, AssetRegistrySearchable)
    EEnvironmentProfileType ProfileType;

    UPROPERTY(EditDefaultsOnly, meta = (UIMin = "0.05", ClampMin = "0.05", UIMax = "30", ClampMax = "30"))
    float TransitionRate = 0.5f;

    UPROPERTY(EditDefaultsOnly, meta = (UIMin = "0.05", ClampMin = "0.05", UIMax = "30", ClampMax = "30"))
    float TransitionDuration = 5.0f;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UCurveFloat> TransitionCurve;


    // ~ UPrimaryDataAsset
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
    // ~ End of UPrimaryDataAsset

    static FPrimaryAssetType GetPrimaryAssetType();

};


/**
 * 
 */
UCLASS(MinimalAPI)
class UEnvironmentFogProfileAsset : public UEnvironmentProfileAsset
{
 
    GENERATED_BODY()
 
public:

    UEnvironmentFogProfileAsset();

    UPROPERTY(EditDefaultsOnly)
    float FogDensity = 0.05f;
 
};


/**
 * 
 */
UCLASS(MinimalAPI)
class UEnvironmentLightProfileAsset : public UEnvironmentProfileAsset
{
 
    GENERATED_BODY()
 
public:

    UEnvironmentLightProfileAsset();

    UPROPERTY(EditDefaultsOnly)
    float SunIntensity = 20.0f;

    UPROPERTY(EditDefaultsOnly)
    FColor SunColor = FColor::White;

    UPROPERTY(EditDefaultsOnly)
    float MoonIntensity = 5.0f;

    UPROPERTY(EditDefaultsOnly)
    FColor MoonColor = FColor::FromHex("#4B6F91");
 
};


/**
 * 
 */
UCLASS(MinimalAPI)
class UEnvironmentAtmosphereProfileAsset : public UEnvironmentProfileAsset
{
 
    GENERATED_BODY()
 
public:

    UEnvironmentAtmosphereProfileAsset();

    UPROPERTY(EditDefaultsOnly)
    float MieScatteringScale = 0.003996f;

};

