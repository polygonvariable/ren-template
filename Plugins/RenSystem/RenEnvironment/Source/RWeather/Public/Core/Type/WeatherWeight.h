// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "WeatherWeight.generated.h"

// Forward Declarations
class UWeatherAsset;


/**
 *
 */
USTRUCT()
struct FWeightedWeatherId
{

    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere)
    TObjectPtr<UWeatherAsset> Asset;

    UPROPERTY(EditAnywhere)
    int Weight = 5;

};

