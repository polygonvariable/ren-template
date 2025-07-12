// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenWeather/Public/WeatherWeight.h"
#include "RenWeather/Public/Actor/WeatherRegionActorBase.h"

// Generated Headers
#include "WeatherSeasonRegionActor.generated.h"

// Forward Declarations
class ISeasonProviderInterface;



/**
 *
 */
UCLASS(Abstract)
class AWeatherSeasonRegionActor : public AWeatherRegionActorBase
{

	GENERATED_BODY()

protected:

	TWeakInterfacePtr<ISeasonProviderInterface> SeasonInterface;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowedClasses = "/Script/RenSeason.SeasonAsset"))
	TMap<TObjectPtr<UPrimaryDataAsset>, FWeatherCollection> WeatherCollections;

protected:

	virtual void HandleWeatherRefreshed() override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

