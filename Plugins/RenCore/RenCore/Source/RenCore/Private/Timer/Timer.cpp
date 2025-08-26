// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Timer/Timer.h"

// Project Headers
#include "RenCoreLibrary/Public/LogMacro.h"


void UTimer::StartTimer(const float InTickInterval, const int InTickLimit, const bool bPreserveTime)
{
	if (InTickInterval < 0.001f)
	{
		LOG_ERROR(LogTemp, "Timer interval cannot be less than 0.001f");
		return;
	}

	TickLimit = InTickLimit;
	TickInterval = InTickInterval;

	if (!bPreserveTime)
	{
		ElapsedTime = 0.0f;
		TickCount = 0;
	}

	if (UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearTimer(TimerHandle);

		//bool bShouldLoop = TickLimit <= 0;
		TimerManager.SetTimer(TimerHandle, this, &UTimer::HandleTick, TickInterval, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
		
		OnStarted.Broadcast();
	}
}


void UTimer::StopTimer(bool bInvalidate)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
		if (bInvalidate) TimerHandle.Invalidate();

		ElapsedTime = 0.0f;
		TickCount = 0;

		OnCompleted.Broadcast();
	}
}

void UTimer::PauseTimer()
{
	if (UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		if (TimerManager.IsTimerActive(TimerHandle))
		{
			TimerManager.PauseTimer(TimerHandle);
		}
	}
}

void UTimer::UnPauseTimer()
{
	if (UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		if (TimerManager.IsTimerPaused(TimerHandle))
		{
			TimerManager.UnPauseTimer(TimerHandle);
		}
	}
}


void UTimer::RestartTimer(const float InTickInterval, const int InTickLimit)
{
	StopTimer();
	StartTimer(InTickInterval, InTickLimit, false);
}


void UTimer::SetTickInterval(const float InTickInterval)
{
	StopTimer();
	StartTimer(InTickInterval, TickLimit, true);
}


const bool UTimer::IsActive()
{
	if (UWorld* World = GetWorld())
	{
		return World->GetTimerManager().IsTimerActive(TimerHandle);
	}
	return false;
}


const float UTimer::GetNormalizedAlpha()
{
	if (TickLimit <= 0 || TickInterval <= 0.0f)
	{
		return 0.0f;
	}
	const float TotalTime = TickLimit * TickInterval;
	return FMath::Clamp(ElapsedTime / TotalTime, 0.0f, 1.0f);
}

void UTimer::HandleTick()
{
	ElapsedTime += TickInterval;
	TickCount++;

	OnTick.Broadcast(ElapsedTime);

	if (TickLimit > 0 && TickCount >= TickLimit)
	{
		StopTimer();
	}
}
