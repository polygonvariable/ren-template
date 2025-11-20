// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenWeather/Public/WeatherDelegate.h"

// Generated Headers
#include "WeatherControllerInterface.generated.h"

// Forward Declarations



UINTERFACE(MinimalAPI)
class UWeatherControllerInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 * 
 * 
 * 
 */
class IWeatherControllerInterface
{

	GENERATED_BODY()

public:

	virtual FWeatherDelegates& GetWeatherDelegates() = 0;

};

