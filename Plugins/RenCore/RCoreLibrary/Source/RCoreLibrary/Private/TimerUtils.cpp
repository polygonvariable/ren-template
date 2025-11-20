// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "TimerUtils.h"
#include "TimerUtils.inl"

// Engine Headers

// Project Headers
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"



bool UTimer::IsActive()
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	return TimerManager.TimerExists(TimerHandle);
}

void UTimer::Start(float InRate, float InDuration)
{
	if (TimerHandle.IsValid())
	{
		LOG_ERROR(LogTimer, TEXT("Timer is already set"));
		return;
	}

	Rate = FMath::Max(0.05f, InRate);
	Duration = FMath::Max(0.0f, InDuration);
	ElapsedTime = 0.0f;

	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	TimerManager.SetTimer(TimerHandle, this, &UTimer::HandleTick, Rate, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
}

void UTimer::Restart(float InRate, float InDuration)
{
	Clear();
	Start(InRate, InDuration);
}

void UTimer::Resume()
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	if (!TimerManager.TimerExists(TimerHandle))
	{
		LOG_ERROR(LogTimer, TEXT("Timer does not exist"));
		return;
	}

	TimerManager.UnPauseTimer(TimerHandle);

	PRINT_INFO(LogTimer, 0.5f, TEXT("Timer Resumed"));
}

void UTimer::Pause()
{
	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	if (!TimerManager.TimerExists(TimerHandle))
	{
		LOG_ERROR(LogTimer, TEXT("Timer does not exist"));
		return;
	}

	TimerManager.PauseTimer(TimerHandle);

	PRINT_INFO(LogTimer, 0.5f, TEXT("Timer Paused"));
}

void UTimer::Clear()
{
	Rate = 0.0f;
	Duration = 0.0f;
	ElapsedTime = 0.0f;

	UWorld* World = GetWorld();
	FTimerManager& TimerManager = World->GetTimerManager();

	TimerManager.ClearTimer(TimerHandle);
	TimerHandle.Invalidate();

	PRINT_INFO(LogTimer, 0.5f, TEXT("Timer Cleared"));
}

void UTimer::HandleTick()
{
	UpdateElapsedTime(Rate);

	OnTick.ExecuteIfBound(ElapsedTime);

	if (ElapsedTime >= Duration)
	{
		OnFinished.ExecuteIfBound();
		Clear();
		return;
	}
}

float UTimer::GetRate() const
{
	return FMath::Max(0.05f, Rate);
}
float UTimer::GetDuration() const
{
	return FMath::Max(0.0f, Duration);
}

float UTimer::GetElapsedTime() const
{
	return ElapsedTime;
}

float UTimer::GetRemainingTime() const
{
	return FMath::Clamp(Duration - ElapsedTime, 0.0f, Duration);
}

void UTimer::UpdateElapsedTime(float DeltaSeconds)
{
	if (Duration <= 0.0f || FMath::IsNearlyZero(DeltaSeconds))
	{
		return;
	}

	ElapsedTime = FMath::Clamp(ElapsedTime + DeltaSeconds, 0.0f, Duration);
}

