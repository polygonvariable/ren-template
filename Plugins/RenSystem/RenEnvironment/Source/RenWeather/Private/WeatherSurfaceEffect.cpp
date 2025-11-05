// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherSurfaceEffect.h"

// Engine Headers
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Header



void FWeatherSurfaceEffect::FillParameters(UMaterialParameterCollectionInstance* Instance, FName WindName, FName RainName, FName SnowName)
{
    Instance->GetScalarParameterValue(WindName, WindStrength);
    Instance->GetScalarParameterValue(RainName, RainIntensity);
    Instance->GetScalarParameterValue(SnowName, SnowIntensity);
}

