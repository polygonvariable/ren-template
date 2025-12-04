// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/OrbitalLightComponent.h"

// Plugins Headers
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

void UOrbitalLightComponent::SetTimeOfDay(float NewTime)
{
	TimeOfDay = NewTime;

	UpdateLightRotation();
    UpdateLightShadow();
}

void UOrbitalLightComponent::UpdateLightRotation()
{
    int Hour = static_cast<int>(TimeOfDay) % 24;
    int Minute = static_cast<int>((TimeOfDay - Hour) * 60.0f) % 60;
    int Second = static_cast<int>(((TimeOfDay - Hour) * 60.0f - Minute) * 60.0f) % 60;

    FSunPositionData SunPositionData;
    USunPositionFunctionLibrary::GetSunPosition(Latitude, Longitude, TimeZone, true, Year, Month, Day, Hour, Minute, Second, SunPositionData);

    float RotationOrientation = bInverseRotation ? -1 : 1;
    float RotationYaw = (SunPositionData.Azimuth + NorthPoleOffset) * RotationOrientation;
    float RotationPitch = SunPositionData.CorrectedElevation * RotationOrientation;

    SetRelativeRotation(FRotator(RotationPitch, RotationYaw, 0.0f));
}

void UOrbitalLightComponent::UpdateLightShadow()
{
    bool bCanCastShadows = IsLightTime(TimeOfDay);

    if (CastShadows != bCanCastShadows)
    {
        SetCastShadows(bCanCastShadows);
    }
}

bool UOrbitalLightComponent::IsLightTime(float InTimeOfDay) const
{
    float Start = FMath::Fmod(EnabledStartTime, 24.0f);
    if (Start < 0.0f) Start += 24.0f;

    float End = FMath::Fmod(EnabledEndTime, 24.0f);
    if (End < 0.0f) End += 24.0f;

    float T = FMath::Fmod(InTimeOfDay, 24.0f);
    if (T < 0.0f) T += 24.0f;

    if (Start < End)
    {
        return (T >= Start && T < End);
    }

    else if (Start > End)
    {
        return (T >= Start || T < End);
    }

    return false;
}

float UOrbitalLightComponent::GetTimeOfDay()
{
	return TimeOfDay;
}

