// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers

// Forward Declarations



/**
 *
 */
class TimerUtils
{

public:

	template <typename T>
	static bool IsValid(FTimerHandle& TimerHandle, T* Object)
	{
		UWorld* World = Object->GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
		
		return TimerManager.TimerExists(TimerHandle);
	}

	template <typename UserClass>
	static bool StartTimer(FTimerHandle& TimerHandle, UserClass* Object, typename FTimerDelegate::TMethodPtr<UserClass> InTimerMethod, float Rate)
	{
		UWorld* World = Object->GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
		if (TimerManager.TimerExists(TimerHandle))
		{
			if (!TimerManager.IsTimerPaused(TimerHandle))
			{
				return false;
			}
			TimerManager.UnPauseTimer(TimerHandle);
		}
		else
		{
			TimerManager.SetTimer(TimerHandle, Object, InTimerMethod, Rate, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
		}

		return true;
	}

	template <typename UserClass>
	static bool ResumeTimer(FTimerHandle& TimerHandle, UserClass* Object)
	{
		UWorld* World = Object->GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
		if (TimerManager.TimerExists(TimerHandle))
		{
			TimerManager.UnPauseTimer(TimerHandle);
			return true;
		}

		return false;
	}

	template <typename UserClass>
	static bool PauseTimer(FTimerHandle& TimerHandle, UserClass* Object)
	{
		UWorld* World = Object->GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
		if (TimerManager.TimerExists(TimerHandle))
		{
			TimerManager.PauseTimer(TimerHandle);
			return true;
		}
		
		return false;
	}

	template <typename UserClass>
	static void ClearTimer(FTimerHandle& TimerHandle, UserClass* Object)
	{
		UWorld* World = Object->GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearAllTimersForObject(Object);
		TimerHandle.Invalidate();
	}

};

