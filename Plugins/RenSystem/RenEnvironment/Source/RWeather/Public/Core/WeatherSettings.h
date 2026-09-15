// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "WeatherSettings.generated.h"

// Forward Declarations
class UWeatherSubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Environment Weather"))
class UWeatherSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UWeatherSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<UWeatherSubsystem> SubsystemClass;

	/*
	 * RGB	- Color added
	 * A	- Color Multiplied
	 */
	UPROPERTY(Config, EditDefaultsOnly, Category = "Surface Property")
	FName SurfaceTint = TEXT("WeatherAttribute.Tint");

	/*
	 * S	- Specular
	 * R	- Roughness
	 * O	- Opacity
	 * W	- WPO
	 */
	UPROPERTY(Config, EditDefaultsOnly, Category = "Surface Property")
	FName SurfaceSROW = TEXT("WeatherAttribute.SROW");

	/*
	 * D	- Displacement
	 * C	- Coverage (The mask coverage)
	 * M	- Mask Type
	 * A	- Mask Add (Add value to mask of Specular & Roughness)
	 */
	UPROPERTY(Config, EditDefaultsOnly, Category = "Surface Property")
	FName SurfaceDCMA = TEXT("WeatherAttribute.DCMA");


	static const UWeatherSettings* Get();

};

