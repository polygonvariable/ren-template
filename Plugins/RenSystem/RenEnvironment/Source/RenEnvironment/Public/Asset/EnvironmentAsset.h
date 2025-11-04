// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers

// Generated Headers
#include "EnvironmentAsset.generated.h"

// Forward Declarations
class UObjectPrioritySystem;
class UEnvironmentProfileAsset;
class UEnvironmentStackedController;
class UEnvironmentDiscreteController;



/**
 *
 */
UCLASS(MinimalAPI)
class UEnvironmentAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly)
    FName ActorTag = TEXT("Actor.Environment");

    UPROPERTY(EditDefaultsOnly)
    TArray<TSubclassOf<UEnvironmentStackedController>> StackedControllers;

	UPROPERTY(EditDefaultsOnly)
    TArray<TSubclassOf<UEnvironmentDiscreteController>> DiscreteControllers;

    UPROPERTY(EditDefaultsOnly, Meta = (AllowedTypes = "Environment.Profile"))
    TArray<FPrimaryAssetId> DefaultProfiles;


    UPROPERTY(EditDefaultsOnly)
    bool bWeatherEnabled = true;

    UPROPERTY(EditDefaultsOnly, Meta = (MetaClass = "WeatherManagerActor"))
    FSoftClassPath WeatherManager;

    UPROPERTY(EditDefaultsOnly, Meta = (AllowedTypes = "Environment.Weather"))
    FPrimaryAssetId DefaultWeather;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UMaterialParameterCollection> WeatherParameterCollection;

    UPROPERTY(EditDefaultsOnly, Meta = (AllowedClasses = "/Script/RenWeather.WeatherController"))
    TSubclassOf<UObjectPrioritySystem> WeatherController;

    UPROPERTY(EditDefaultsOnly)
    float WeatherRefreshDuration = 5.0f;


    UPROPERTY(EditDefaultsOnly)
    bool bSeasonEnabled = true;

    UPROPERTY(EditDefaultsOnly, Meta = (AllowedTypes = "Environment.Season"))
    TArray<FPrimaryAssetId> DefaultSeasons;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UMaterialParameterCollection> SeasonParameterCollection;

#if WITH_EDITOR

    // ~ UObject
    virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const
    {
        bool bAnyFailed = false;

        /*
        if (bWeatherEnabled)
        {
            if (!WeatherManager.IsValid())
            {
                Context.AddError(FText::FromString(TEXT("Weather Manager is invalid.")));
				bAnyFailed = true;
            }

            if (!DefaultWeather.IsValid())
            {
                Context.AddError(FText::FromString(TEXT("Default Weather is invalid.")));
				bAnyFailed = true;
            }

            if (!WeatherParameterCollection)
            {
                Context.AddError(FText::FromString(TEXT("Weather Parameter Collection is invalid.")));
				bAnyFailed = true;
            }

            if (!WeatherController)
            {
                Context.AddError(FText::FromString(TEXT("Weather Controller is invalid.")));
                bAnyFailed = true;
            }

            if (WeatherRefreshDuration <= 1.0f)
            {
                Context.AddError(FText::FromString(TEXT("Weather Refresh Duration must be greater than 1.")));
				bAnyFailed = true;
            }
        }*/

        EDataValidationResult Result = (bAnyFailed) ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
        return Result;
    }
    // ~ End of UObject

#endif

};

