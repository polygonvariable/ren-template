// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/WeatherSettings.h"

// Project Headers
#include "System/WeatherSubsystem.h"


UWeatherSettings::UWeatherSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	SubsystemClass = UWeatherSubsystem::StaticClass();
}

const UWeatherSettings* UWeatherSettings::Get()
{
	return GetDefault<UWeatherSettings>();
}

