// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "WeatherSettings.generated.h"


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Environment Weather"))
class UWeatherSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UWeatherSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly, Category = "Surface Property")
	FName SurfaceTint = TEXT("WeatherA.Tint");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Surface Property")
	FName SurfaceSROW = TEXT("WeatherA.SROW");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Surface Effect")
	FName EffectRain = TEXT("WeatherE.Rain");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Surface Effect")
	FName EffectSnow = TEXT("WeatherE.Snow");

	
	static const UWeatherSettings* Get();

};

