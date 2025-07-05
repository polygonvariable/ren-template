// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "EnvironmentProfile.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FEnvironmentProfile
{

    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Priority = -1;

};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEnvironmentFogProfile : public FEnvironmentProfile
{
 
    GENERATED_BODY()
 
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FogDensity = 0.05f;
 
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEnvironmentLightProfile : public FEnvironmentProfile
{
 
    GENERATED_BODY()
 
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SunIntensity = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FColor SunColor = FColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoonIntensity = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FColor MoonColor = FColor::White;
 
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEnvironmentAtmosphereProfile : public FEnvironmentProfile
{
 
    GENERATED_BODY()
 
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MieScatteringScale = 0.003996f;

};

