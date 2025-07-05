// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Project Headers
#include "RenEnvironment/Public/Profile/WeatherWeight.h"

// Generated Headers
#include "SeasonAsset.generated.h"



/**
 *
 */
UCLASS()
class USeasonAsset : public UPrimaryDataAsset
{

    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly)
    FName SeasonName = NAME_None;



    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "1", ClampMin = "1"))
    int SeasonStartDay = 0;

	UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "1", ClampMin = "1"))
	int SeasonEndDay = 10;



    UPROPERTY(EditDefaultsOnly)
    UCurveFloat* SeasonCurve = nullptr;



    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
    float MaterialSpecular = 0.0f;

    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
    float MaterialRoughness = 0.0f;

    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
    float MaterialOpacity = 0.0f;

    UPROPERTY(EditDefaultsOnly)
    FColor MaterialColor = FColor::White;



    UPROPERTY(EditDefaultsOnly)
    TArray<FWeatherWeight> WeatherWeights;

};

