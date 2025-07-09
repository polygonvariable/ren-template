// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Forward Declarations
class UWeatherAsset;

// Delegate Declarations
DECLARE_MULTICAST_DELEGATE(FOnWeatherRefreshed);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeatherChanged, UWeatherAsset* /* WeatherAsset */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeatherRemoved, UWeatherAsset* /* WeatherAsset */);

