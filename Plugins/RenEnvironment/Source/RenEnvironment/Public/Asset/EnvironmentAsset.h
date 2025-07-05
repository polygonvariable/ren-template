// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Project Headers
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"

// Generated Headers
#include "EnvironmentAsset.generated.h"


// Forward Declarations
class UWeatherAsset;
class USeasonAsset;
class UEnvironmentStackedController;
class UEnvironmentDiscreteController;



/**
 *
 */
UCLASS()
class UEnvironmentAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly)
    TMap<TEnumAsByte<EEnvironmentProfileType>, TSubclassOf<UEnvironmentStackedController>> StackedControllers;

	UPROPERTY(EditDefaultsOnly)
	TSet<TSubclassOf<UEnvironmentDiscreteController>> DiscreteControllers;



    UPROPERTY(EditDefaultsOnly)
    bool bEnableWeather = true;

    UPROPERTY(EditDefaultsOnly)
    UWeatherAsset* DefaultWeather;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UMaterialParameterCollection> WeatherMaterialParameter;



    UPROPERTY(EditDefaultsOnly)
    bool bEnableSeason = true;

    UPROPERTY(EditDefaultsOnly)
    TArray<USeasonAsset*> DefaultSeasons;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UMaterialParameterCollection> SeasonMaterialParameter;

};

