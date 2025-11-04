// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherController.h"

// Engine Headers
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Header
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreLibrary/Public/TimerUtils.h"
#include "RCoreMaterial/Public/MaterialLibrary.h"

#include "RenEnvironment/Public/Subsystem/EnvironmentSubsystem.h"



void UWeatherController::SetMaterialCollection(UMaterialParameterCollectionInstance* MaterialCollection)
{
	MPC = MaterialCollection;
}


void UWeatherController::TimerUpdate(float Rate, float Duration)
{
	PRINT_INFO(LogWeather, 0.1f, TEXT("Weather Update"));
}

void UWeatherController::AddEnvironmentProfile(UWeatherAsset* WeatherAsset)
{
	UWorld* World = GetWorld();
	UEnvironmentSubsystem* EnvironmentSubsystem = World->GetSubsystem<UEnvironmentSubsystem>();

	if (!IsValid(EnvironmentSubsystem) || !IsValid(WeatherAsset))
	{
		LOG_ERROR(LogWeather, TEXT("EnvironmentSubsystem, WeatherAsset is invalid"));
		return;
	}

	int Priority = WeatherAsset->ProfilePriority;

	TArray<TPair<FPrimaryAssetId, FGuid>>& LatentIds = LatentCollection.FindOrAdd(WeatherAsset);
	const TArray<FPrimaryAssetId>& Profiles = WeatherAsset->EnvironmentProfiles;

	for (const FPrimaryAssetId& AssetId : Profiles)
	{
		FGuid LatentId = FGuid::NewGuid();
		LatentIds.Add({ AssetId, LatentId });

		EnvironmentSubsystem->AddProfile(AssetId, Priority);
	}
}

void UWeatherController::RemoveEnvironmentProfile(UWeatherAsset* WeatherAsset)
{
	UWorld* World = GetWorld();
	UEnvironmentSubsystem* EnvironmentSubsystem = World->GetSubsystem<UEnvironmentSubsystem>();

	if (!IsValid(EnvironmentSubsystem) || !IsValid(WeatherAsset))
	{
		LOG_ERROR(LogWeather, TEXT("EnvironmentSubsystem, WeatherAsset is invalid"));
		return;
	}

	int Priority = WeatherAsset->ProfilePriority;

	TArray<TPair<FPrimaryAssetId, FGuid>> LatentIds;
	LatentCollection.RemoveAndCopyValue(WeatherAsset, LatentIds);

	for (const TPair<FPrimaryAssetId, FGuid>& Kv : LatentIds)
	{
		const FPrimaryAssetId& ProfileId = Kv.Key;
		const FGuid& LatentId = Kv.Value;
		EnvironmentSubsystem->RemoveProfile(LatentId, ProfileId, Priority);
	}
}


void UWeatherController::HandleTransition(const FMaterialSurfaceProperty& SurfaceProperty, const FWeatherSurfaceEffect& SurfaceEffect)
{
	/*
	CurrentSurfaceProperty.GetParameters(MPC, TEXT("WeatherTint"), TEXT("WeatherSpecular"), TEXT("WeatherRoughness"), TEXT("WeatherOpacity"));
	CurrentSurfaceEffect.GetParameters(MPC, TEXT("WeatherWind"), TEXT("WeatherRain"), TEXT("WeatherSnow"));

	TargetSurfaceProperty = SurfaceProperty;
	TargetSurfaceEffect = SurfaceEffect;

	if (IsValid(WeatherTimer))
	{
		WeatherTimer->RestartTimer();
	}
	*/

	MaterialLibrary::LerpScalarParameter(MPC, TEXT("WeatherSpecular"), SurfaceProperty.Specular, 1.0f);
	MaterialLibrary::LerpScalarParameter(MPC, TEXT("WeatherRoughness"), SurfaceProperty.Roughness, 1.0f);
	MaterialLibrary::LerpScalarParameter(MPC, TEXT("WeatherOpacity"), SurfaceProperty.Opacity, 1.0f);
	MaterialLibrary::LerpVectorParameter(MPC, TEXT("WeatherTint"), SurfaceProperty.Tint, 1.0f);

	MaterialLibrary::LerpScalarParameter(MPC, TEXT("WeatherWind"), SurfaceEffect.WindStrength, 1.0f);
	MaterialLibrary::LerpScalarParameter(MPC, TEXT("WeatherRain"), SurfaceEffect.RainIntensity, 1.0f);
	MaterialLibrary::LerpScalarParameter(MPC, TEXT("WeatherSnow"), SurfaceEffect.SnowIntensity, 1.0f);
}

void UWeatherController::Initialize()
{
	if (!IsValid(WeatherTimer))
	{
		WeatherTimer = NewObject<UTimer>(this);
		WeatherTimer->OnTimerUpdate.BindUObject(this, &UWeatherController::TimerUpdate);
	}
}

void UWeatherController::Deinitialize()
{
	UWeatherAsset* WeatherAsset = CurrentWeather.Get();
	RemoveEnvironmentProfile(WeatherAsset);

	if (IsValid(WeatherTimer))
	{
		WeatherTimer->OnTimerUpdate.Unbind();
		WeatherTimer->ClearTimer();
	}
	WeatherTimer = nullptr;

	LatentCollection.Empty();
	CurrentWeather = nullptr;
	MPC = nullptr;

	Super::Deinitialize();
}


void UWeatherController::HandleItemChanged(UObject* Item)
{
	UWeatherAsset* WeatherAsset = Cast<UWeatherAsset>(Item);
	if (!MPC || !IsValid(WeatherAsset) || WeatherAsset == CurrentWeather)
	{
		LOG_ERROR(LogWeather, TEXT("MPC, WeatherAsset is invalid or already active"));
		return;
	}

	CurrentWeather = WeatherAsset;

	AddEnvironmentProfile(WeatherAsset);

	FMaterialSurfaceProperty& SurfaceProperty = WeatherAsset->SurfaceProperty;
	FWeatherSurfaceEffect& SurfaceEffect = WeatherAsset->SurfaceEffect;
	HandleTransition(SurfaceProperty, SurfaceEffect);

	LOG_INFO(LogWeather, TEXT("Weather changed"));
	Delegates.OnChanged.Broadcast(WeatherAsset);
}

void UWeatherController::HandleItemRemoved(UObject* Item, bool bWasReplaced)
{
	UWeatherAsset* WeatherAsset = Cast<UWeatherAsset>(Item);
	if (!IsValid(WeatherAsset))
	{
		LOG_ERROR(LogWeather, TEXT("WeatherAsset is invalid"));
		return;
	}

	RemoveEnvironmentProfile(WeatherAsset);

	LOG_INFO(LogWeather, TEXT("Weather removed/replaced"));
	Delegates.OnRemoved.Broadcast(WeatherAsset);
}

void UWeatherController::HandleNoItemsLeft()
{
	CurrentWeather = nullptr;

	LOG_ERROR(LogWeather, TEXT("Weather controller has no items left, which was not supposed to happen"));
}




bool UTimer::IsTimerValid()
{
	return TimerUtils::IsValid(TimerHandle, this);
}

void UTimer::SetTimer(float InRate, float InDuration)
{
	if (TimerHandle.IsValid())
	{
		LOG_ERROR(LogWeather, TEXT("Timer is already set"));
		return;
	}

	Rate = FMath::Max(0.05f, InRate);
	Duration = FMath::Max(0.05f, InDuration);
	ElapsedTime = 0.0f;

	TimerUtils::StartTimer(TimerHandle, this, &UTimer::TimerTick, Rate);
}

void UTimer::RestartTimer()
{
	ElapsedTime = 0.0f;
	ResumeTimer();
}

void UTimer::ResumeTimer()
{
	TimerUtils::ResumeTimer(TimerHandle, this);
	LOG_INFO(LogWeather, TEXT("Timer Resumed"));
}

void UTimer::PauseTimer()
{
	TimerUtils::PauseTimer(TimerHandle, this);
	LOG_INFO(LogWeather, TEXT("Timer Paused"));
}

void UTimer::ClearTimer()
{
	Rate = 0.0f;
	Duration = 0.0f;
	ElapsedTime = 0.0f;

	TimerUtils::ClearTimer(TimerHandle, this);
	LOG_INFO(LogWeather, TEXT("Timer Cleared"));
}

void UTimer::TimerTick()
{
	float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);
	OnTimerUpdate.ExecuteIfBound(ElapsedTime, Alpha);

	ElapsedTime += Rate;

	if (ElapsedTime >= Duration)
	{
		ElapsedTime = 0.0f;
		PauseTimer();
		return;
	}

	LOG_INFO(LogWeather, TEXT("Timer Tick"));
}

