// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/EnvironmentController.h"

// Project Header
#include "Data/EnvironmentProfileAsset.h"


void UEnvironmentDiscreteController::Initialize(AActor* Actor)
{
}

void UEnvironmentDiscreteController::Deinitialize()
{
}


void UEnvironmentStackedController::Initialize(AActor* Actor)
{

}

void UEnvironmentStackedController::Deinitialize()
{
	ClearPriorityItems();
	ClearTransition();
}

bool UEnvironmentStackedController::AddProfile(UEnvironmentProfileAsset* Profile, int Priority)
{
	return AddPriorityItem(Profile, Priority);
}

bool UEnvironmentStackedController::RemoveProfile(int Priority)
{
	return RemovePriorityItem(Priority);
}


TMap<int, TWeakObjectPtr<UObject>>& UEnvironmentStackedController::GetPriorityItems()
{
	return _PriorityItems;
}

int& UEnvironmentStackedController::GetHighestPriority()
{
	return _HighestPriority;
}

void UEnvironmentStackedController::OnPriorityItemChanged(UObject* Item)
{
	const UEnvironmentProfileAsset* Profile = Cast<UEnvironmentProfileAsset>(Item);
	if (IsValid(Profile))
	{
		_TransitionRate = FMath::Max(0.05f, Profile->TransitionRate);
		_TransitionDuration = FMath::Clamp(Profile->TransitionDuration, 0.05f, 30.0f);
		TransitionCurve = Profile->TransitionCurve;
	}
}


void UEnvironmentStackedController::StartTransition()
{
	ClearTransition();

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle, this, &UEnvironmentStackedController::HandleOnTransitionTick, _TransitionRate, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
}

void UEnvironmentStackedController::ClearTransition()
{
	_ElapsedTime = 0.0f;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(TimerHandle);
	TimerHandle.Invalidate();
}

void UEnvironmentStackedController::OnTransitionChanged(float Alpha)
{

}

void UEnvironmentStackedController::HandleOnTransitionTick()
{
	if (_TransitionDuration <= 0.0f || FMath::IsNearlyZero(_TransitionRate))
	{
		ClearTransition();
		return;
	}

	_ElapsedTime = FMath::Clamp(_ElapsedTime + _TransitionRate, 0.0f, _TransitionDuration);

	OnTransitionChanged(FMath::Clamp(_ElapsedTime / _TransitionDuration, 0.0f, 1.0f));

	if (_ElapsedTime >= _TransitionDuration)
	{
		ClearTransition();
		return;
	}
}

