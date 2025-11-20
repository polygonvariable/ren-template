// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherController.h"

// Engine Headers
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Header
#include "RCoreCommon/Public/Priority/PriorityList.h"

#include "RCoreLibrary/Private/TimerUtils.inl"
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreMaterial/Public/MaterialLibrary.h"

#include "RenEnvironment/Public/Subsystem/EnvironmentSubsystem.h"
#include "RenWeather/Public/WeatherAsset.h"



void UWeatherController::Initialize()
{
	if (IsValid(PriorityList))
	{
		LOG_ERROR(LogWeather, TEXT("PriorityList is already valid"));
		return;
	}

	PriorityList = NewObject<UPriorityList>(this);
	if (!IsValid(PriorityList))
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create PriorityList"));
		return;
	}

	FPriorityListDelegates& PriorityDelegates = PriorityList->GetPriorityDelegates();
	PriorityDelegates.OnChanged.BindUObject(this, &UWeatherController::HandleItemChanged);
	PriorityDelegates.OnRemoved.BindUObject(this, &UWeatherController::HandleItemRemoved);
	PriorityDelegates.OnCleared.BindUObject(this, &UWeatherController::HandleItemCleared);

	UWorld* World = GetWorld();
	EnvironmentSubsystem = World->GetSubsystem<UEnvironmentSubsystem>();

	Timer = NewObject<UTimer>(this);
	if (!IsValid(Timer))
	{
		LOG_ERROR(LogWeather, TEXT("Failed to create Timer"));
		return;
	}
	Timer->OnTick.BindUObject(this, &UWeatherController::HandleTimerTick);
}

void UWeatherController::Deinitialize()
{
	UWeatherAsset* WeatherAsset = CurrentWeather.Get();
	RemoveEnvironmentProfile(WeatherAsset);

	if (IsValid(Timer))
	{
		Timer->Clear();
		Timer->OnTick.Unbind();
		Timer->MarkAsGarbage();
	}
	Timer = nullptr;

	if (IsValid(PriorityList))
	{
		FPriorityListDelegates& PriorityDelegates = PriorityList->GetPriorityDelegates();
		PriorityDelegates.ClearAll();

		PriorityList->CleanUpItems();
		PriorityList->MarkAsGarbage();
	}
	PriorityList = nullptr;

	MPC = nullptr;
	CurrentWeather = nullptr;
	LatentCollection.Empty();

	EnvironmentSubsystem.Reset();
}

void UWeatherController::SetMaterialCollection(UMaterialParameterCollectionInstance* MaterialCollection)
{
	MPC = MaterialCollection;
}

bool UWeatherController::AddWeather(UWeatherAsset* WeatherAsset, int Priority)
{
	if (!IsValid(PriorityList) || !IsValid(WeatherAsset))
	{
		LOG_ERROR(LogWeather, TEXT("PriorityList, WeatherAsset is invalid"));
		return false;
	}

	return PriorityList->AddItem(WeatherAsset, Priority);
}

bool UWeatherController::RemoveWeather(int Priority)
{
	if (!IsValid(PriorityList))
	{
		LOG_ERROR(LogWeather, TEXT("PriorityList is invalid"));
		return false;
	}

	return PriorityList->RemoveItem(Priority);
}

FWeatherDelegates& UWeatherController::GetWeatherDelegates()
{
	return Delegates;
}

void UWeatherController::RefreshWeather()
{
	PRINT_INFO(LogWeather, 1.0f, TEXT("Weather refreshed"));
	Delegates.OnRefreshed.Broadcast();
}


void UWeatherController::AddEnvironmentProfile(UWeatherAsset* WeatherAsset)
{
	UEnvironmentSubsystem* Environment = EnvironmentSubsystem.Get();
	if (!IsValid(Environment) || !IsValid(WeatherAsset))
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

		Environment->AddProfile(LatentId, AssetId, Priority);
	}
}

void UWeatherController::RemoveEnvironmentProfile(UWeatherAsset* WeatherAsset)
{
	UEnvironmentSubsystem* Environment = EnvironmentSubsystem.Get();
	if (!IsValid(Environment) || !IsValid(WeatherAsset))
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

		Environment->RemoveProfile(LatentId, ProfileId, Priority);
	}
}


void UWeatherController::StartTransition(UWeatherAsset* WeatherAsset)
{
	if (!IsValid(WeatherAsset) || !IsValid(MPC) || !IsValid(Timer))
	{
		LOG_ERROR(LogWeather, TEXT("WeatherAsset, MPC, Timer is invalid"));
		return;
	}

	TargetSurfaceProperty = WeatherAsset->SurfaceProperty;
	TargetSurfaceEffect = WeatherAsset->SurfaceEffect;

	SourceSurfaceProperty.Reset();
	SourceSurfaceEffect.Reset();

	SourceSurfaceProperty.GetParameters(MPC, TEXT("WeatherTint"), TEXT("WeatherSpecular"), TEXT("WeatherRoughness"), TEXT("WeatherOpacity"));
	SourceSurfaceEffect.GetParameters(MPC, TEXT("WeatherWind"), TEXT("WeatherRain"), TEXT("WeatherSnow"));

	float TransitionRate = WeatherAsset->TransitionRate;
	float TransitionDuration = WeatherAsset->TransitionDuration;

	Timer->Restart(TransitionRate, TransitionDuration);
}

void UWeatherController::HandleTimerTick(float ElapsedTime)
{
	if (!MPC)
	{
		return;
	}

	float Duration = Timer->GetDuration();
	float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

	FMaterialSurfaceProperty SurfaceProperty = FMaterialSurfaceProperty::Lerp(SourceSurfaceProperty, TargetSurfaceProperty, Alpha);
	FWeatherSurfaceEffect SurfaceEffect = FWeatherSurfaceEffect::Lerp(SourceSurfaceEffect, TargetSurfaceEffect, Alpha);

	SurfaceProperty.SetParameters(MPC, TEXT("WeatherTint"), TEXT("WeatherSpecular"), TEXT("WeatherRoughness"), TEXT("WeatherOpacity"));
	SurfaceEffect.SetParameters(MPC, TEXT("WeatherWind"), TEXT("WeatherRain"), TEXT("WeatherSnow"));

	PRINT_INFO(LogEnvironment, 5.0f, TEXT("Elapsed: %f, Duration: %f, Alpha: %f"), ElapsedTime, Duration, Alpha);
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
	StartTransition(WeatherAsset);

	LOG_INFO(LogWeather, TEXT("Weather changed"));
	Delegates.OnChanged.Broadcast(WeatherAsset);
}

void UWeatherController::HandleItemRemoved(UObject* Item, bool bReplaced)
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

void UWeatherController::HandleItemCleared()
{
	CurrentWeather = nullptr;

	LOG_ERROR(LogWeather, TEXT("Weather controller has no items left, which was not supposed to happen"));
}

