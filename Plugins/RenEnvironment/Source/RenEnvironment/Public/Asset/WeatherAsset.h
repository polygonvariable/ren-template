// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Generated Headers
#include "WeatherAsset.generated.h"

// Forward Declarations
class UEnvironmentProfileAsset;


/**
 *
 */
UCLASS()
class UWeatherAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly)
    FName WeatherName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "0", UIMax = "1", ClampMin = "0", ClampMax = "1"))
    float MaterialAlpha = 0.0f;

    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
    float MaterialSpecular = 0.0f;

    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
    float MaterialRoughness = 0.0f;

    UPROPERTY(EditDefaultsOnly, Meta = (UIMin = "-1", UIMax = "1", ClampMin = "-1", ClampMax = "1"))
    float MaterialOpacity = 0.0f;

    UPROPERTY(EditDefaultsOnly)
    FColor MaterialColor = FColor::Transparent;


    /*
        Key: Environment Profile Asset
		Value: Priority of the profile (make sure to use same priority for multiple profiles)
    */
    UPROPERTY(EditDefaultsOnly)
    TMap<TObjectPtr<UEnvironmentProfileAsset>, int> EnvironmentProfiles;

};

