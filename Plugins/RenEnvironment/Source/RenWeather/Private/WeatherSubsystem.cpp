// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherSubsystem.h"

// Engine Header

// Project Header
#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"

#include "RenEnvironment/Public/Asset/EnvironmentAsset.h"
#include "RenEnvironment/Public/EnvironmentWorldSettings.h"

#include "RenWeather/Public/WeatherAsset.h"
#include "RenWeather/Public/WeatherController.h"



bool UWeatherSubsystem::AddWeather(UWeatherAsset* WeatherAsset, int Priority)
{
	if (!IsValid(WeatherController) || !IsValid(WeatherAsset))
	{
		LOG_ERROR(LogTemp, "WeatherController or WeatherAsset is not valid");
		return false;
	}
	return WeatherController->AddItem(WeatherAsset, Priority);
}

bool UWeatherSubsystem::RemoveWeather(int Priority)
{
	if (!IsValid(WeatherController))
	{
		LOG_ERROR(LogTemp, "WeatherController is not valid");
		return false;
	}
	return WeatherController->RemoveItem(Priority);
}



FOnWeatherChanged& UWeatherSubsystem::GetOnWeatherChanged()
{
	return WeatherController->OnWeatherChanged;
}
FOnWeatherRemoved& UWeatherSubsystem::GetOnWeatherRemoved()
{
	return WeatherController->OnWeatherRemoved;
}


void UWeatherSubsystem::CreateWeatherTimer(float RefreshTime)
{
	TimerUtils::StartTimer(WeatherTimer, this, &UWeatherSubsystem::HandleWeatherTimer, FMath::Max(RefreshTime, 1.0f));
}


bool UWeatherSubsystem::CreateWeatherController()
{
	if (IsValid(WeatherController))
	{
		LOG_WARNING(LogTemp, "Weather Controller is already valid");
		return false;
	}

	if (!IsValid(EnvironmentAsset) || !IsValid(EnvironmentAsset->WeatherController))
	{
		LOG_ERROR(LogTemp, " EnvironmentAsset, WeatherController Class is not valid");
		return false;
	}

	WeatherController = NewObject<UWeatherController>(this, EnvironmentAsset->WeatherController);
	if (!IsValid(WeatherController))
	{
		LOG_ERROR(LogTemp, "Failed to create Weather Controller");
		return false;
	}

	return true;
}


void UWeatherSubsystem::CreateWeatherMaterialCollection()
{
	if (!IsValid(EnvironmentAsset) || !IsValid(WeatherController))
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentAsset or WeatherController is invalid"));
		return;
	}
	WeatherController->SetMaterialCollection(EnvironmentAsset->WeatherMaterialParameter);
}


void UWeatherSubsystem::HandleWeatherTimer()
{
	PRINT_WARNING(LogTemp, 1.0f, TEXT("Weather changed"));
	OnWeatherRefresh.Broadcast();
}

bool UWeatherSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UWeatherSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("WeatherSubsystem initialized"));
}

void UWeatherSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);
	LOG_WARNING(LogTemp, TEXT("WeatherSubsystem components updated"));

	AEnvironmentWorldSettings* WorldSettings = Cast<AEnvironmentWorldSettings>(InWorld.GetWorldSettings());
	if (!IsValid(WorldSettings))
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentWorldSettings is not valid"));
		return;
	}

	EnvironmentAsset = WorldSettings->EnvironmentAsset;
	if (!IsValid(WorldSettings->EnvironmentAsset))
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentAsset is not valid"));
		return;
	}

	if (CreateWeatherController())
	{
		CreateWeatherTimer(EnvironmentAsset->WeatherRefreshDuration);
		CreateWeatherMaterialCollection();

		UWeatherAsset* WeatherAsset = Cast<UWeatherAsset>(EnvironmentAsset->DefaultWeather);
		if (IsValid(WeatherAsset))
		{
			AddWeather(WeatherAsset, 0);
		}
	}
}

void UWeatherSubsystem::Deinitialize()
{
	if (IsValid(WeatherController))
	{
		WeatherController->CleanUpItems();
		WeatherController->MarkAsGarbage();
	}
	WeatherController = nullptr;
	EnvironmentAsset = nullptr;

	TimerUtils::ClearTimer(WeatherTimer, this);

	LOG_WARNING(LogTemp, TEXT("WeatherSubsystem deinitialized"));
	Super::Deinitialize();
}

