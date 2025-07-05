// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/OrbitalLightComponent.h"
#include "SunPosition/Public/SunPosition.h"


UOrbitalLightComponent::UOrbitalLightComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;

	Intensity = 20.0f;
	LightSourceAngle = 4.0f;

	FarShadowCascadeCount = 1;
	DynamicShadowCascades = 2;
	DynamicShadowDistanceMovableLight = 4000.0f;
}

void UOrbitalLightComponent::SetTime(float NewTime)
{
	Time = NewTime;

	int Hour = static_cast<int>(Time) % 24;
	int Minute = static_cast<int>((Time - Hour) * 60.0f) % 60;
	int Second = static_cast<int>(((Time - Hour) * 60.0f - Minute) * 60.0f) % 60;

	FSunPositionData SunPositionData;
	USunPositionFunctionLibrary::GetSunPosition(Latitude, Longitude, TimeZone, true, Year, Month, Day, Hour, Minute, Second, SunPositionData);

	float RotationOrientation = bInverseRotation ? -1 : 1;
	float RotationYaw = (SunPositionData.Azimuth + NorthPoleOffset) * RotationOrientation;
	float RotationPitch = SunPositionData.CorrectedElevation * RotationOrientation;

	SetRelativeRotation(FRotator(RotationPitch, RotationYaw, 0.0f));
}

float UOrbitalLightComponent::GetTime()
{
	return Time;
}

