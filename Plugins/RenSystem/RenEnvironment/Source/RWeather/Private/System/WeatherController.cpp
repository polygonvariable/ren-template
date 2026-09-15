// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/WeatherController.h"

// Engine Headers
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Header
#include "Core/WeatherSettings.h"
#include "Data/WeatherAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "MaterialLibrary.h"
#include "System/EnvironmentSubsystem.h"


void UWeatherController::Initialize(UMaterialParameterCollectionInstance* InMPCInstance)
{
	MPCInstance = InMPCInstance;
	EnvironmentSubsystem = UEnvironmentSubsystem::Get(GetWorld());
}

void UWeatherController::Deinitialize()
{
	TransitionCurve = nullptr;

	OnWeatherChanged.Clear();
	OnWeatherRemoved.Clear();

	RemoveEnvironmentProfile(CurrentWeather);

	ClearPriorityItems();
	ClearTransition();

	MPCInstance = nullptr;
	CurrentWeather = nullptr;
	EnvironmentSubsystem = nullptr;
}


bool UWeatherController::AddWeather(UWeatherAsset* WeatherAsset, int Priority)
{
	return AddPriorityItem(WeatherAsset, Priority);
}

bool UWeatherController::RemoveWeather(int Priority)
{
	return RemovePriorityItem(Priority);
}



#if UE_BUILD_DEVELOPMENT
float UWeatherController::GetEditorWeatherTransition() const
{
	return _ElapsedTime;
}
int UWeatherController::GetEditorCurrentWeatherPriority() const
{
	return _HighestPriority;
}
const FString UWeatherController::GetEditorCurrentWeatherName() const
{
	return IsValid(CurrentWeather) ? CurrentWeather->GetFName().ToString() : TEXT("Invalid Asset");
}
const UWeatherAsset* UWeatherController::GetEditorCurrentWeather() const
{
	return CurrentWeather;
}
const TMap<int, TWeakObjectPtr<UObject>>& UWeatherController::GetEditorWeathers() const
{
	return _PriorityItems;
}
#endif


void UWeatherController::AddEnvironmentProfile()
{
	if (!IsValid(EnvironmentSubsystem) || !IsValid(CurrentWeather))
	{
		LOG_ERROR(LogWeather, TEXT("EnvironmentSubsystem, WeatherAsset is invalid"));
		return;
	}

	const TArray<TObjectPtr<UEnvironmentProfileAsset>>& Profiles = CurrentWeather->EnvironmentProfiles;
	for (UEnvironmentProfileAsset* Item : Profiles)
	{
		EnvironmentSubsystem->AddProfile(Item, CurrentWeather->ProfilePriority);
	}
}

void UWeatherController::RemoveEnvironmentProfile(UWeatherAsset* WeatherAsset)
{
	if (!IsValid(EnvironmentSubsystem) || !IsValid(WeatherAsset))
	{
		LOG_ERROR(LogWeather, TEXT("EnvironmentSubsystem, WeatherAsset is invalid"));
		return;
	}

	const TArray<TObjectPtr<UEnvironmentProfileAsset>>& Profiles = WeatherAsset->EnvironmentProfiles;
	for (UEnvironmentProfileAsset* Item : Profiles)
	{
		EnvironmentSubsystem->RemoveProfile(Item, WeatherAsset->ProfilePriority);
	}
}


void UWeatherController::StartTransition()
{
	ClearTransition();

	PRINT_INFO(LogWeather, 5.0f, TEXT("Weather transition started"));

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(TimerHandle, this, &UWeatherController::HandleOnTransitionTick, _TransitionRate, FTimerManagerTimerParameters{ .bLoop = true, .bMaxOncePerFrame = true });
}

void UWeatherController::ClearTransition()
{
	PRINT_INFO(LogWeather, 5.0f, TEXT("Weather transition stopped"));

	_ElapsedTime = 0.0f;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(TimerHandle);
	TimerHandle.Invalidate();
}

void UWeatherController::OnTransitionChanged(float Alpha)
{
	if (!IsValid(MPCInstance) || !IsValid(TransitionCurve))
	{
		return;
	}

	const UWeatherSettings* Settings = UWeatherSettings::Get();
	float Curve = TransitionCurve->GetFloatValue(Alpha);

	FMaterialSurfaceProperty SurfaceProperty;
	FMaterialLibrary::LerpSurfaceProperty(SourceSurfaceProperty, TargetSurfaceProperty, Curve, SurfaceProperty);
	FMaterialLibrary::SetSurfaceProperty(SurfaceProperty, MPCInstance, Settings->SurfaceTint, Settings->SurfaceSROW, Settings->SurfaceDCMA);
}

void UWeatherController::HandleOnTransitionTick()
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


TMap<int, TWeakObjectPtr<UObject>>& UWeatherController::GetPriorityItems()
{
	return _PriorityItems;
}

int& UWeatherController::GetHighestPriority()
{
	return _HighestPriority;
}

void UWeatherController::OnPriorityItemChanged(UObject* Item)
{
	UWeatherAsset* WeatherAsset = Cast<UWeatherAsset>(Item);
	if (!IsValid(MPCInstance) || !IsValid(WeatherAsset) || WeatherAsset == CurrentWeather)
	{
		LOG_ERROR(LogWeather, TEXT("MPC, WeatherAsset is invalid or already active"));
		return;
	}

	CurrentWeather = WeatherAsset;
	TransitionCurve = WeatherAsset->TransitionCurve;

	TargetSurfaceProperty = CurrentWeather->SurfaceProperty;
	SourceSurfaceProperty.Reset();

	const UWeatherSettings* Settings = UWeatherSettings::Get();

	FMaterialLibrary::GetSurfaceProperty(SourceSurfaceProperty, MPCInstance, Settings->SurfaceTint, Settings->SurfaceSROW, Settings->SurfaceDCMA);

	_TransitionRate = CurrentWeather->TransitionRate;
	_TransitionDuration = CurrentWeather->TransitionDuration;

	AddEnvironmentProfile();
	StartTransition();

	LOG_INFO(LogWeather, TEXT("Weather changed"));
	OnWeatherChanged.Broadcast(WeatherAsset);
}

void UWeatherController::OnPriorityItemRemoved(UObject* Item, bool bReplaced)
{
	UWeatherAsset* WeatherAsset = Cast<UWeatherAsset>(Item);
	if (!IsValid(WeatherAsset))
	{
		LOG_ERROR(LogWeather, TEXT("WeatherAsset is invalid"));
		return;
	}

	RemoveEnvironmentProfile(WeatherAsset);

	LOG_INFO(LogWeather, TEXT("Weather removed/replaced"));
	OnWeatherRemoved.Broadcast(WeatherAsset);
}

void UWeatherController::OnPriorityItemCleared()
{
	CurrentWeather = nullptr;

	LOG_ERROR(LogWeather, TEXT("Weather controller has no items left, which was not supposed to happen"));
}

