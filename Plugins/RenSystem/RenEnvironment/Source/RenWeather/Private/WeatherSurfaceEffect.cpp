// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherSurfaceEffect.h"

// Engine Headers
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Header



void FWeatherSurfaceEffect::Reset()
{
	WindIntensity = 0.0f;
	RainIntensity = 0.0f;
	SnowIntensity = 0.0f;
}

void FWeatherSurfaceEffect::Clamp()
{
	WindIntensity = FMath::Clamp(WindIntensity, 0.0f, 1.0f);
	RainIntensity = FMath::Clamp(RainIntensity, 0.0f, 1.0f);
	SnowIntensity = FMath::Clamp(SnowIntensity, 0.0f, 1.0f);
}

void FWeatherSurfaceEffect::GetParameters(UMaterialParameterCollectionInstance* Instance, FName WindName, FName RainName, FName SnowName)
{
    if (!Instance) return;

    Instance->GetScalarParameterValue(WindName, WindIntensity);
    Instance->GetScalarParameterValue(RainName, RainIntensity);
    Instance->GetScalarParameterValue(SnowName, SnowIntensity);
}

void FWeatherSurfaceEffect::SetParameters(UMaterialParameterCollectionInstance* Instance, FName WindName, FName RainName, FName SnowName)
{
	if (!Instance) return;

	Instance->SetScalarParameterValue(WindName, WindIntensity);
	Instance->SetScalarParameterValue(RainName, RainIntensity);
	Instance->SetScalarParameterValue(SnowName, SnowIntensity);
}

FWeatherSurfaceEffect FWeatherSurfaceEffect::Lerp(const FWeatherSurfaceEffect& A, const FWeatherSurfaceEffect& B, float Alpha)
{
	FWeatherSurfaceEffect Result;

	Result.WindIntensity = FMath::Lerp(A.WindIntensity, B.WindIntensity, Alpha);
	Result.RainIntensity = FMath::Lerp(A.RainIntensity, B.RainIntensity, Alpha);
	Result.SnowIntensity = FMath::Lerp(A.SnowIntensity, B.SnowIntensity, Alpha);

	Result.Clamp();

	return Result;
}

