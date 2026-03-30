// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers
#include "TimerUtil.generated.h"

// Module Macros
#define REN_API RCORELIBRARY_API

// Forward Declarations



/**
 * 
 * 
 * 
 */
class TimerUtil
{

public:

	template <typename T>
	static bool IsValid(FTimerHandle& TimerHandle, T* Object)
	{
		UWorld* World = Object->GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();

		return TimerManager.TimerExists(TimerHandle);
	}

	template <typename T>
	static bool StartTimer(FTimerHandle& TimerHandle, T* Object, typename FTimerDelegate::TMethodPtr<T> InTimerMethod, float Rate)
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

	template <typename T>
	static bool ResumeTimer(FTimerHandle& TimerHandle, T* Object)
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

	template <typename T>
	static bool PauseTimer(FTimerHandle& TimerHandle, T* Object)
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

	template <typename T>
	static void ClearTimer(FTimerHandle& TimerHandle, T* Object)
	{
		UWorld* World = Object->GetWorld();
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.ClearAllTimersForObject(Object);
		TimerHandle.Invalidate();
	}

};



/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class UTimer : public UObject
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE_OneParam(FTimerTick, float /*ElapsedTime*/);
	FTimerTick OnTick;

	DECLARE_DELEGATE(FTimerFinished);
	FTimerFinished OnFinished;

	REN_API bool IsActive();


	REN_API void Start(float InRate, float InDuration);

	REN_API void Restart(float InRate, float InDuration);
	REN_API void Resume();
	REN_API void Pause();
	REN_API void Clear();

	REN_API float GetRate() const;
	REN_API float GetDuration() const;
	REN_API float GetElapsedTime() const;
	REN_API float GetRemainingTime() const;

	REN_API void UpdateElapsedTime(float DeltaSeconds);

protected:

	float Rate = 0.0f;
	float Duration = 0.0f;
	float ElapsedTime = 0.0f;

	void HandleTick();

private:

	FTimerHandle TimerHandle;

};



// Module Macros
#undef REN_API

