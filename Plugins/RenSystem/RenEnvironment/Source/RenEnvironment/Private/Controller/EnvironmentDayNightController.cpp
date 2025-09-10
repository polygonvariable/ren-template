// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Controller/EnvironmentDayNightController.h"

// Engine Headers
#include "EngineUtils.h"

// Project Headers
#include "RCoreClock/Public/ClockManagerInterface.h"

#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreLibrary/Public/SubsystemUtils.h"
#include "RCoreLibrary/Public/TimerUtils.h"

#include "RenEnvironment/Public/Component/OrbitalLightComponent.h"



bool UEnvironmentDayNightController::LoadComponents()
{
	bool bSunFound = false;
	bool bMoonFound = false;

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (IsValid(*ActorItr) && ActorItr->ActorHasTag(ActorTag))
		{
			TArray<UActorComponent*> SunComponents = ActorItr->GetComponentsByTag(UOrbitalLightComponent::StaticClass(), SunComponentTag);
			if (SunComponents.IsValidIndex(0) && IsValid(SunComponents[0]))
			{
				SunComponent = Cast<UOrbitalLightComponent>(SunComponents[0]);
				bSunFound = true;
			}

			TArray<UActorComponent*> MoonComponents = ActorItr->GetComponentsByTag(UOrbitalLightComponent::StaticClass(), MoonComponentTag);
			if (MoonComponents.IsValidIndex(0) && IsValid(MoonComponents[0]))
			{
				MoonComponent = Cast<UOrbitalLightComponent>(MoonComponents[0]);
				bMoonFound = true;
			}

			break;
		}
	}

	return bSunFound && bMoonFound;
}

void UEnvironmentDayNightController::StartDayTimer()
{
	if (!SunComponent.IsValid() || !MoonComponent.IsValid())
	{
		LOG_ERROR(LogTemp, TEXT("Sun or Moon not found"));
		return;
	}

	if (!TimerUtils::StartTimer(DayTimerHandle, this, &UEnvironmentDayNightController::HandleDayTimerTick, 0.1f))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to create timer or timer is already running"));
	}
}

void UEnvironmentDayNightController::StopDayTimer()
{
	if (!TimerUtils::PauseTimer(DayTimerHandle, this))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to pause timer"));
	}
}

void UEnvironmentDayNightController::HandleDayTimerTick()
{
	float NormalizedTime = ClockManager->GetSmoothNormalizedTime();
	float RealTime = NormalizedTime * 24.0f;

	SunComponent->SetTime(RealTime);
	MoonComponent->SetTime(RealTime);
}



void UEnvironmentDayNightController::InitializeController()
{
	if (!LoadComponents())
	{
		LOG_ERROR(LogTemp, TEXT("Failed to find components"));
		return;
	}

	if (IClockManagerInterface* ClockManagerInterface = SubsystemUtils::GetSubsystemInterface<UWorld, UWorldSubsystem, IClockManagerInterface>(GetWorld()))
	{
		FClockDelegates& ClockDelegate = ClockManagerInterface->GetClockDelegates();
		ClockDelegate.OnClockStarted.AddUObject(this, &UEnvironmentDayNightController::StartDayTimer);
		ClockDelegate.OnClockStopped.AddUObject(this, &UEnvironmentDayNightController::StopDayTimer);

		if (ClockManager->IsClockActive())
		{
			StartDayTimer();
		}

		ClockManager = TWeakInterfacePtr<IClockManagerInterface>(ClockManagerInterface);
	}
}

void UEnvironmentDayNightController::CleanupController()
{
	if (IClockManagerInterface* ClockManagerInterface = ClockManager.Get())
	{
		FClockDelegates& ClockDelegate = ClockManagerInterface->GetClockDelegates();
		ClockDelegate.OnClockStarted.RemoveAll(this);
		ClockDelegate.OnClockStopped.RemoveAll(this);
	}
	ClockManager.Reset();

	TimerUtils::ClearTimer(DayTimerHandle, this);

	SunComponent.Reset();
	MoonComponent.Reset();
}

