// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Controller/EnvironmentController.h"

// Project Header
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/TimerUtils.h"

#include "RCoreCommon/Public/Priority/PriorityList.h"
#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"



void UEnvironmentDiscreteController::Initialize(AActor* Actor)
{
}

void UEnvironmentDiscreteController::Deinitialize()
{
}


void UEnvironmentStackedController::Initialize(AActor* Actor)
{
	if (IsValid(PriorityList) || !IsValid(Actor))
	{
		return;
	}

	PriorityList = NewObject<UPriorityList>(this);
	if (IsValid(PriorityList))
	{
		FPriorityListDelegates& PriorityDelegates = PriorityList->GetPriorityDelegates();
		PriorityDelegates.OnChanged.BindUObject(this, &UEnvironmentStackedController::HandleItemChanged);
	}

	if (!IsValid(Timer))
	{
		Timer = NewObject<UTimer>(this);
		if (!IsValid(Timer))
		{
			LOG_ERROR(LogWeather, TEXT("Failed to create Timer"));
			return;
		}
		Timer->OnTick.BindUObject(this, &UEnvironmentStackedController::HandleTimerTick);
	}
}

void UEnvironmentStackedController::Deinitialize()
{
	if (IsValid(PriorityList))
	{
		FPriorityListDelegates& PriorityDelegates = PriorityList->GetPriorityDelegates();
		PriorityDelegates.ClearAll();

		PriorityList->CleanUpItems();
		PriorityList->MarkAsGarbage();
	}
	PriorityList = nullptr;

	if (IsValid(Timer))
	{
		Timer->Clear();
		Timer->OnTick.Unbind();
		Timer->MarkAsGarbage();
	}
	Timer = nullptr;
}

bool UEnvironmentStackedController::AddProfile(UEnvironmentProfileAsset* Profile, int Priority)
{
	if (!IsValid(PriorityList))
	{
		return false;
	}
	return PriorityList->AddItem(Profile, Priority);
}

bool UEnvironmentStackedController::RemoveProfile(int Priority)
{
	if (!IsValid(PriorityList))
	{
		return false;
	}
	return PriorityList->RemoveItem(Priority);
}

void UEnvironmentStackedController::HandleItemChanged(UObject* Item)
{
	UEnvironmentProfileAsset* Profile = Cast<UEnvironmentProfileAsset>(Item);
	if (IsValid(Profile))
	{
		SetTransitionRate(Profile->TransitionRate);
		SetTransitionDuration(Profile->TransitionDuration);
	}
}

void UEnvironmentStackedController::StartTransition()
{
	if (IsValid(Timer))
	{
		Timer->Restart(TransitionRate, TransitionDuration);
	}
}

void UEnvironmentStackedController::SetTransitionRate(float InRate)
{
	TransitionRate = FMath::Max(0.05f, InRate);
}

void UEnvironmentStackedController::SetTransitionDuration(float InDuration)
{
	TransitionDuration = FMath::Clamp(InDuration, 0.05f, 30.0f);
}

float UEnvironmentStackedController::GetTransitionDuration() const
{
	return TransitionDuration;
}

void UEnvironmentStackedController::HandleTimerTick(float ElapsedTime)
{

}

