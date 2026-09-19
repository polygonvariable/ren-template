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
    float TransitionRate = 0.2f;

    UPROPERTY(EditDefaultsOnly, meta = (UIMin = "0.05", ClampMin = "0.05", UIMax = "30", ClampMax = "30"))
    float TransitionDuration = 4.0f;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UCurveFloat> TransitionCurve;


    // ~ UPrimaryDataAsset
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
    // ~ End of UPrimaryDataAsset

#if WITH_EDITORONLY_DATA
    // ~ UPrimaryDataAsset
    virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
    // ~ End of UPrimaryDataAsset
#endif

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
    float SunIntensity = 10.0f;

    UPROPERTY(EditDefaultsOnly)
    FLinearColor SunColor = FColor::White;

    UPROPERTY(EditDefaultsOnly)
    float SunRadius = 4.0f;

    UPROPERTY(EditDefaultsOnly)
    float MoonIntensity = 5.0f;

    UPROPERTY(EditDefaultsOnly)
    FLinearColor MoonColor = FColor::FromHex("#4B6F91");

    UPROPERTY(EditDefaultsOnly)
    float MoonRadius = 0.0f;

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

    UPROPERTY(EditDefaultsOnly)
    float MieAnisotropy = 0.8f;

};

