// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"



class TimerUtils
{

public:

	template <typename UserClass>
	static bool StartTimer(FTimerHandle& TimerHandle, UserClass* Object, typename FTimerDelegate::TMethodPtr<UserClass> InTimerMethod, float Rate)
	{
		FTimerManager& TimerManager = Object->GetWorld()->GetTimerManager();
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
	static bool PauseTimer(FTimerHandle& TimerHandle, UserClass* Object)
	{
		FTimerManager& TimerManager = Object->GetWorld()->GetTimerManager();
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
		FTimerManager& TimerManager = Object->GetWorld()->GetTimerManager();
		TimerManager.ClearAllTimersForObject(Object);
		TimerHandle.Invalidate();
	}

};

