// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherSubsystem.h"

// Engine Header

// Project Header
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreLibrary/Public/TimerUtils.h"
#include "RCoreSettings/Public/WorldConfigSettings.h"

#include "RenEnvironment/Public/Asset/EnvironmentAsset.h"

#include "RenWeather/Public/WeatherAsset.h"
#include "RenWeather/Public/WeatherController.h"



bool UWeatherSubsystem::AddWeather(UWeatherAsset* WeatherAsset, int Priority)
{
	if (!IsValid(WeatherController) || !IsValid(WeatherAsset))
	{
		LOG_ERROR(LogTemp, TEXT("WeatherController or WeatherAsset is invalid"));
		return false;
	}
	return WeatherController->AddItem(WeatherAsset, Priority);
}

bool UWeatherSubsystem::RemoveWeather(int Priority)
{
	if (!IsValid(WeatherController))
	{
		LOG_ERROR(LogTemp, TEXT("WeatherController is invalid"));
		return false;
	}
	return WeatherController->RemoveItem(Priority);
}


bool UWeatherSubsystem::CreateWeatherTimer(float RefreshTime)
{
	return TimerUtils::StartTimer(WeatherTimer, this, &UWeatherSubsystem::HandleWeatherTimer, FMath::Max(5.0f, RefreshTime));
}


bool UWeatherSubsystem::CreateWeatherController(TSubclassOf<UObjectPrioritySystem> ControllerClass)
{
	if (IsValid(WeatherController) || !IsValid(ControllerClass))
	{
		LOG_ERROR(LogTemp, TEXT("WeatherController is already valid or WeatherController Class is invalid"));
		return false;
	}

	UWeatherController* Controller = NewObject<UWeatherController>(this, ControllerClass);
	if (!IsValid(Controller))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to create WeatherController"));
		return false;
	}
	Controller->OnWeatherChanged.AddWeakLambda(this, [this](UWeatherAsset* WeatherAsset) { OnWeatherChanged.Broadcast(WeatherAsset); });
	Controller->OnWeatherRemoved.AddWeakLambda(this, [this](UWeatherAsset* WeatherAsset) { OnWeatherRemoved.Broadcast(WeatherAsset); });

	WeatherController = Controller;

	return true;
}


void UWeatherSubsystem::CreateWeatherMaterialCollection(UMaterialParameterCollection* Collection)
{
	if (!IsValid(WeatherController) || !Collection)
	{
		LOG_ERROR(LogTemp, TEXT("WeatherController or MaterialParameterCollection is invalid"));
		return;
	}
	WeatherController->SetMaterialCollection(Collection);
}


void UWeatherSubsystem::HandleWeatherTimer()
{
	PRINT_WARNING(LogTemp, 1.0f, TEXT("Weather changed"));
	OnWeatherRefreshed.Broadcast();
}

bool UWeatherSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UWeatherSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("WeatherSubsystem Initialize"));
}

void UWeatherSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_WARNING(LogTemp, TEXT("WeatherSubsystem OnWorldComponentsUpdated"));

	AWorldConfigSettings* WorldSettings = Cast<AWorldConfigSettings>(InWorld.GetWorldSettings());
	if (!IsValid(WorldSettings))
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentWorldSettings is invalid"));
		return;
	}

	UEnvironmentAsset* EnvironmentAsset = Cast<UEnvironmentAsset>(WorldSettings->EnvironmentAsset);
	if (!IsValid(EnvironmentAsset))
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentAsset is invalid"));
		return;
	}

	if (!EnvironmentAsset->bEnableWeather)
	{
		LOG_ERROR(LogTemp, TEXT("Weather is disabled"));
		return;
	}

	if (!IsValid(EnvironmentAsset->WeatherController))
	{
		LOG_ERROR(LogTemp, TEXT("WeatherController is invalid"));
		return;
	}

	if (!EnvironmentAsset->WeatherMaterialParameter)
	{
		LOG_ERROR(LogTemp, TEXT("WeatherMaterialParameter is invalid"));
		return;
	}

	if (!CreateWeatherController(EnvironmentAsset->WeatherController))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to create WeatherController"));
		return;
	}

	CreateWeatherMaterialCollection(EnvironmentAsset->WeatherMaterialParameter);
	CreateWeatherTimer(EnvironmentAsset->WeatherRefreshDuration);

	UWeatherAsset* DefaultWeather = Cast<UWeatherAsset>(EnvironmentAsset->DefaultWeather);
	if (IsValid(DefaultWeather))
	{
		AddWeather(DefaultWeather, 0);
	}
}

void UWeatherSubsystem::Deinitialize()
{
	if (IsValid(WeatherController))
	{
		WeatherController->OnWeatherChanged.RemoveAll(this);
		WeatherController->OnWeatherRemoved.RemoveAll(this);
		WeatherController->CleanUpItems();
		WeatherController->MarkAsGarbage();
	}
	WeatherController = nullptr;

	TimerUtils::ClearTimer(WeatherTimer, this);

	LOG_WARNING(LogTemp, TEXT("WeatherSubsystem Deinitialized"));
	Super::Deinitialize();
}

