// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Clock/ClockSubsystem.h"

// Project Headers
#include "Clock/ClockWorldConfig.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "WorldFragmentSettings.h"


bool UClockSubsystem::GetSmoothNormalizedTime(float& Time) const
{
	return 0.0f;
}

bool UClockSubsystem::IsClockActive() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(ClockHandle);
}

bool UClockSubsystem::GetDayLength(int& Length) const
{
	if (!IsValid(ClockConfig))
	{
		return false;
	}
	Length = ClockConfig->DayLength;
	return true;
}

bool UClockSubsystem::GetYearLength(int& Length) const
{
	if (!IsValid(ClockConfig))
	{
		return false;
	}
	Length = ClockConfig->YearLength;
	return true;
}

int UClockSubsystem::GetCurrentTime() const
{
	return CurrentTime;
}

int UClockSubsystem::GetCurrentDay() const
{
	return CurrentDay;
}

int UClockSubsystem::GetCurrentYear() const
{
	return CurrentYear;
}


void UClockSubsystem::CreateClockTimer()
{
	float TickTime = FMath::Clamp(ClockConfig->TickInterval, 0.05f, 10.0f);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(ClockHandle, this, &UClockSubsystem::HandleOnClockTick, TickTime, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
	
	ClockStateChanged.Broadcast(true);
}

void UClockSubsystem::RemoveClockTimer()
{
	ClockStateChanged.Broadcast(false);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(ClockHandle);
	ClockHandle.Invalidate();
}

void UClockSubsystem::HandleOnClockTick()
{
	CurrentTime++;
	if (CurrentTime > ClockConfig->DayLength)
	{
		CurrentTime = 0;
		CurrentDay++;

		if (CurrentDay > ClockConfig->YearLength)
		{
			CurrentDay = 1;
			CurrentYear++;
		}

		ClockDayChanged.Broadcast(CurrentDay);
	}

	ClockTimeChanged.Broadcast(CurrentTime);
}


bool UClockSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

bool UClockSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}
	const UClockWorldConfig* Config = AWorldFragmentSettings::GetConfigByClass<UClockWorldConfig>(Cast<UWorld>(Outer));
	return IsValid(Config) && Config->bEnabled;
}

void UClockSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("ClockSubsystem Initialized"));
}

void UClockSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	LOG_WARNING(LogTemp, TEXT("ClockSubsystem OnWorldComponentsUpdated"));

	ClockConfig = AWorldFragmentSettings::GetConfigByClass<UClockWorldConfig>(&InWorld);
	if (!IsValid(ClockConfig) || !ClockConfig->bEnabled)
	{
		LOG_ERROR(LogWeather, TEXT("Clock config is invalid or disabled"));
		return;
	}

	CreateClockTimer();
}

void UClockSubsystem::OnWorldEndPlay(UWorld& InWorld)
{
	ClockConfig = nullptr;
	RemoveClockTimer();

	LOG_WARNING(LogTemp, TEXT("ClockSubsystem OnWorldEndPlay"));
}

UClockSubsystem* UClockSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return World->GetSubsystem<UClockSubsystem>();
}

