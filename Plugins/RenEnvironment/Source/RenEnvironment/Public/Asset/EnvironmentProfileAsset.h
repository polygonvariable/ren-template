// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Project Headers
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"

// Generated Headers
#include "EnvironmentProfileAsset.generated.h"



/**
 * 
 */
UCLASS()
class UEnvironmentProfileAsset : public UPrimaryDataAsset
{

    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly)
    TEnumAsByte<EEnvironmentProfileType> ProfileType;

};



/**
 * 
 */
UCLASS()
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
UCLASS()
class UEnvironmentLightProfileAsset : public UEnvironmentProfileAsset
{
 
    GENERATED_BODY()
 
public:

    UEnvironmentLightProfileAsset();



    UPROPERTY(EditDefaultsOnly)
    float SunIntensity = 10.0f;

    UPROPERTY(EditDefaultsOnly)
    FColor SunColor = FColor::White;

    UPROPERTY(EditDefaultsOnly)
    float MoonIntensity = 10.0f;

    UPROPERTY(EditDefaultsOnly)
    FColor MoonColor = FColor::White;
 
};



/**
 * 
 */
UCLASS()
class UEnvironmentAtmosphereProfileAsset : public UEnvironmentProfileAsset
{
 
    GENERATED_BODY()
 
public:

    UEnvironmentAtmosphereProfileAsset();



    UPROPERTY(EditDefaultsOnly)
    float MieScatteringScale = 0.003996f;

};

