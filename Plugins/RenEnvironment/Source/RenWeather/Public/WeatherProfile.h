// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "WeatherProfile.generated.h"



/**
 *
 */
USTRUCT(BlueprintType)
struct FWeatherProfile
{

    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Alpha = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Specular = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Roughness = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Opacity = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FColor Color = FColor::White;

};

