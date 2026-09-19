// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Controller/EnvironmentDayNightController.h"

// Project Headers
#include "ClockManagerInterface.h"
#include "Component/OrbitalLightComponent.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Util/SubsystemUtil.h"


void UEnvironmentDayNightController::Initialize(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		LOG_ERROR(LogEnvironment, TEXT("Actor is invalid"));
		return;
	}

	SunComponent = Actor->FindComponentByTag<UOrbitalLightComponent>(TEXT("Environment.Sun"));
	MoonComponent = Actor->FindComponentByTag<UOrbitalLightComponent>(TEXT("Environment.Moon"));

	if (!SunComponent.IsValid() || !MoonComponent.IsValid())
	{
		LOG_ERROR(LogEnvironment, TEXT("Sun, moon component is invalid"));
		return;
	}

	ClockManager = FSubsystemLibrary::GetSubsystemInterface<IClockManagerInterface>(GetWorld());
	if (ClockManager)
	{
		ClockManager->OnClockTimeChanged().AddUObject(this, &UEnvironmentDayNightController::HandleOnTimeChanged);
	}
}

void UEnvironmentDayNightController::Deinitialize()
{
	if (ClockManager)
	{
		ClockManager->OnClockTimeChanged().RemoveAll(this);
	}
	ClockManager = nullptr;

	SunComponent.Reset();
	MoonComponent.Reset();
}

void UEnvironmentDayNightController::HandleOnTimeChanged(int Time)
{
	UOrbitalLightComponent* Sun = SunComponent.Get();
	UOrbitalLightComponent* Moon = MoonComponent.Get();

	if (IsValid(Sun) && IsValid(Moon))
	{
		float NormalizedTime = ClockManager->GetNormalizedTime();
		float RealTime = NormalizedTime * 24.0f;

		SunComponent->SetTimeOfDay(RealTime);
		MoonComponent->SetTimeOfDay(RealTime);
	}
}

