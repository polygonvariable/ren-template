// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "WeatherWeight.generated.h"

// Forward Declarations
class UWeatherAsset;



/**
 *
 */
USTRUCT(BlueprintType)
struct FWeatherWeight
{

    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UWeatherAsset* WeatherAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Weight = 0;

};


