// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Headers
#include "RCoreMaterial/Public/MaterialSurfaceProperty.h"

// Generated Headers
#include "WeatherAsset.generated.h"

// Forward Declarations
class UEnvironmentProfileAsset;
class AWeatherEffectActor;



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
    float WindStrength = 0.0f;

    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "0", UIMax = "1", ClampMin = "0", ClampMax = "1"))
    float RainIntensity = 0.0f;

    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "0", UIMax = "1", ClampMin = "0", ClampMax = "1"))
    float SnowIntensity = 0.0f;

    void GetParameters(UMaterialParameterCollectionInstance* Instance, FName WindName, FName RainName, FName SnowName)
    {
        Instance->GetScalarParameterValue(WindName, WindStrength);
		Instance->GetScalarParameterValue(RainName, RainIntensity);
		Instance->GetScalarParameterValue(SnowName, SnowIntensity);
    }

};


/**
 *
 * 
 */
UCLASS()
class UWeatherAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly)
    FName WeatherName = NAME_None;

    UPROPERTY(EditDefaultsOnly)
    FMaterialSurfaceProperty SurfaceProperty;

    UPROPERTY(EditDefaultsOnly)
    FWeatherSurfaceEffect SurfaceEffect;


    UPROPERTY(EditDefaultsOnly, Meta = (AllowedTypes = "Environment.Profile"))
    TArray<FPrimaryAssetId> EnvironmentProfiles;

    UPROPERTY(EditDefaultsOnly)
    int ProfilePriority = 0;


    UPROPERTY(EditDefaultsOnly)
    TArray<TSoftClassPtr<AWeatherEffectActor>> EffectClasses;


    // ~ UPrimaryDataAsset
    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(GetPrimaryAssetType(), GetFName());
    }
    // ~ End of UPrimaryDataAsset

    static FPrimaryAssetType GetPrimaryAssetType()
    {
        return FPrimaryAssetType(TEXT("Environment.Weather"));
    }

    static FPrimaryAssetId MakePrimaryAssetId(const FName& AssetName)
    {
        return FPrimaryAssetId(GetPrimaryAssetType(), AssetName);
    }

    static bool IsValid(const FPrimaryAssetId& AssetId)
    {
        return AssetId.PrimaryAssetType == GetPrimaryAssetType();
    }

};

