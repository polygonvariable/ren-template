// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenWeather/Public/WeatherWeight.h"
#include "RenWeather/Public/Actor/WeatherRegionActorBase.h"

// Generated Headers
#include "WeatherRegionActor.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(Abstract)
class AWeatherRegionActor : public AWeatherRegionActorBase
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWeatherCollection WeatherCollection;

protected:

	virtual void HandleWeatherRefreshed() override;

};

