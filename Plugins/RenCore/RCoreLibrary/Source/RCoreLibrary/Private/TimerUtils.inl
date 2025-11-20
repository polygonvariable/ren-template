// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "TimerUtils.h"

// Engine Headers

// Project Headers

/*
template <typename T>
bool TimerUtils::IsValid(FTimerHandle& TimerHandle, T* Object)
{
    UWorld* World = Object->GetWorld();
    FTimerManager& TimerManager = World->GetTimerManager();
    
    return TimerManager.TimerExists(TimerHandle);
}

template <typename T>
bool TimerUtils::StartTimer(FTimerHandle& TimerHandle, T* Object, typename FTimerDelegate::TMethodPtr<T> InTimerMethod, float Rate)
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
bool TimerUtils::ResumeTimer(FTimerHandle& TimerHandle, T* Object)
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
bool TimerUtils::PauseTimer(FTimerHandle& TimerHandle, T* Object)
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
void TimerUtils::ClearTimer(FTimerHandle& TimerHandle, T* Object)
{
    UWorld* World = Object->GetWorld();
    FTimerManager& TimerManager = World->GetTimerManager();
    TimerManager.ClearAllTimersForObject(Object);
    TimerHandle.Invalidate();
}
*/
