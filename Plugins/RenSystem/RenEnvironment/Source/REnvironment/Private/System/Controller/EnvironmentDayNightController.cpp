// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Controller/EnvironmentDayNightController.h"

// Engine Headers
#include "EngineUtils.h"

// Project Headers
#include "ClockManagerInterface.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Util/SubsystemUtil.h"
#include "RCoreLibrary/Private/Util/TimerUtil.inl"
#include "Component/OrbitalLightComponent.h"



void UEnvironmentDayNightController::StartDayTimer()
{
	if (!SunComponent.IsValid() || !MoonComponent.IsValid())
	{
		LOG_ERROR(LogEnvironment, TEXT("Sun or Moon not found"));
		return;
	}

	bool bResult = TimerUtil::StartTimer(DayTimerHandle, this, &UEnvironmentDayNightController::HandleDayTimerTick, 0.1f);
	if (!bResult)
	{
		LOG_ERROR(LogEnvironment, TEXT("Failed to create timer or timer is already running"));
	}
}

void UEnvironmentDayNightController::StopDayTimer()
{
	if (!TimerUtil::PauseTimer(DayTimerHandle, this))
	{
		LOG_ERROR(LogEnvironment, TEXT("Failed to pause timer"));
	}
}

void UEnvironmentDayNightController::HandleDayTimerTick()
{
	float NormalizedTime = ClockManagerInterface->GetSmoothNormalizedTime();
	float RealTime = NormalizedTime * 24.0f;

	SunComponent->SetTimeOfDay(RealTime);
	MoonComponent->SetTimeOfDay(RealTime);
}



void UEnvironmentDayNightController::Initialize(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		LOG_ERROR(LogEnvironment, TEXT("Actor is invalid"));
		return;
	}

	IClockManagerInterface* ClockManager = SubsystemUtil::GetSubsystemInterface<UWorld, UWorldSubsystem, IClockManagerInterface>(GetWorld());

	SunComponent = Actor->GetComponentByClass<UOrbitalLightComponent>();
	MoonComponent = Actor->GetComponentByClass<UOrbitalLightComponent>();

	if (!ClockManager || !SunComponent.IsValid() || !MoonComponent.IsValid())
	{
		LOG_ERROR(LogEnvironment, TEXT("ClockManager, Sun, Moon is invalid"));
		return;
	}

	FClockDelegates& ClockDelegate = ClockManager->GetClockDelegates();
	ClockDelegate.OnClockStarted.AddUObject(this, &UEnvironmentDayNightController::StartDayTimer);
	ClockDelegate.OnClockStopped.AddUObject(this, &UEnvironmentDayNightController::StopDayTimer);

	if (ClockManager->IsClockActive())
	{
		StartDayTimer();
	}

	ClockManagerInterface = TWeakInterfacePtr<IClockManagerInterface>(ClockManager);
}

void UEnvironmentDayNightController::Deinitialize()
{
	IClockManagerInterface* ClockManager = ClockManagerInterface.Get();
	if (ClockManager)
	{
		FClockDelegates& ClockDelegate = ClockManager->GetClockDelegates();
		ClockDelegate.OnClockStarted.RemoveAll(this);
		ClockDelegate.OnClockStopped.RemoveAll(this);
	}
	ClockManagerInterface.Reset();

	TimerUtil::ClearTimer(DayTimerHandle, this);

	SunComponent.Reset();
	MoonComponent.Reset();
}

