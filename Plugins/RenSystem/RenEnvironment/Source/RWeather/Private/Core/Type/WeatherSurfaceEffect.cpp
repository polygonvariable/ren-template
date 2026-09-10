// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/Type/WeatherSurfaceEffect.h"

// Engine Headers
#include "Core/WeatherSettings.h"
#include "Materials/MaterialParameterCollectionInstance.h"


void FWeatherSurfaceEffect::Reset()
{
	RainIntensity = 0.0f;
	SnowIntensity = 0.0f;
}

void FWeatherSurfaceEffect::Clamp()
{
	RainIntensity = FMath::Clamp(RainIntensity, 0.0f, 1.0f);
	SnowIntensity = FMath::Clamp(SnowIntensity, 0.0f, 1.0f);
}

void FWeatherSurfaceEffect::GetParameters(UMaterialParameterCollectionInstance* MPCInstance)
{
	if (!IsValid(MPCInstance))
	{
		return;
	}

	const UWeatherSettings* Settings = UWeatherSettings::Get();

	MPCInstance->GetScalarParameterValue(Settings->EffectRain, RainIntensity);
	MPCInstance->GetScalarParameterValue(Settings->EffectSnow, SnowIntensity);
}

void FWeatherSurfaceEffect::SetParameters(UMaterialParameterCollectionInstance* MPCInstance)
{
	if (!IsValid(MPCInstance))
	{
		return;
	}

	const UWeatherSettings* Settings = UWeatherSettings::Get();

	MPCInstance->SetScalarParameterValue(Settings->EffectRain, RainIntensity);
	MPCInstance->SetScalarParameterValue(Settings->EffectSnow, SnowIntensity);
}

FWeatherSurfaceEffect FWeatherSurfaceEffect::Lerp(const FWeatherSurfaceEffect& A, const FWeatherSurfaceEffect& B, float Alpha)
{
	FWeatherSurfaceEffect Result;

	Result.RainIntensity = FMath::Lerp(A.RainIntensity, B.RainIntensity, Alpha);
	Result.SnowIntensity = FMath::Lerp(A.SnowIntensity, B.SnowIntensity, Alpha);

	Result.Clamp();

	return Result;
}

