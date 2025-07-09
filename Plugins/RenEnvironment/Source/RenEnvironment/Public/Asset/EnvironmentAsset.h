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
class USeasonAsset;
class UEnvironmentProfileAsset;
class UEnvironmentStackedController;
class UEnvironmentDiscreteController;
class UObjectPrioritySystem;



/**
 *
 */
UCLASS()
class UEnvironmentAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly)
    TSet<TSubclassOf<UEnvironmentStackedController>> StackedControllers;

	UPROPERTY(EditDefaultsOnly)
	TSet<TSubclassOf<UEnvironmentDiscreteController>> DiscreteControllers;



    UPROPERTY(EditDefaultsOnly)
    TSet<TObjectPtr<UEnvironmentProfileAsset>> DefaultStackedProfiles;



    UPROPERTY(EditDefaultsOnly)
    bool bEnableWeather = true;

    UPROPERTY(EditDefaultsOnly, Meta = (AllowedClasses = "/Script/RenWeather.WeatherAsset"))
    TObjectPtr<UPrimaryDataAsset> DefaultWeather;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UMaterialParameterCollection> WeatherMaterialParameter;

    UPROPERTY(EditDefaultsOnly, Meta = (AllowedClasses = "/Script/RenWeather.WeatherController"))
    TSubclassOf<UObjectPrioritySystem> WeatherController;

    UPROPERTY(EditDefaultsOnly)
    float WeatherRefreshDuration = 5.0f;


    UPROPERTY(EditDefaultsOnly)
    bool bEnableSeason = true;

    UPROPERTY(EditDefaultsOnly)
    TArray<USeasonAsset*> DefaultSeasons;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UMaterialParameterCollection> SeasonMaterialParameter;

};

