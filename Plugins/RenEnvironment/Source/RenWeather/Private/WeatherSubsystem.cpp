// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherSubsystem.h"

// Engine Header

// Project Header
#include "RenCore/Public/Developer/GameMetadataSettings.h"
#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"

#include "RenEnvironment/Public/Asset/EnvironmentAsset.h"
#include "RenEnvironment/Public/Asset/WeatherAsset.h"
#include "RenEnvironment/Public/EnvironmentWorldSettings.h"

#include "RenWeather/Public/WeatherController.h"



void UWeatherSubsystem::AddWeather(UWeatherAsset* WeatherAsset, int Priority)
{
	if (!IsValid(WeatherController) || !IsValid(WeatherAsset))
	{
		LOG_ERROR(LogTemp, "WeatherController or WeatherAsset is not valid");
		return;
	}
	WeatherController->AddItem(WeatherAsset, Priority);
}

void UWeatherSubsystem::RemoveWeather(int Priority)
{
	if (!IsValid(WeatherController))
	{
		LOG_ERROR(LogTemp, "WeatherController is not valid");
		return;
	}
	WeatherController->RemoveItem(Priority);
}



void UWeatherSubsystem::CreateWeatherTimer()
{
	TimerUtils::StartTimer(WeatherTimerHandle, this, &UWeatherSubsystem::HandleWeatherTimer, 10.0f);
}

void UWeatherSubsystem::HandleWeatherTimer()
{
	PRINT_WARNING(LogTemp, 1.0f, TEXT("Weather can change"));
	OnWeatherChanged.Broadcast();
}



bool UWeatherSubsystem::CreateWeatherController()
{
	if (IsValid(WeatherController))
	{
		LOG_WARNING(LogTemp, "Weather Controller is already valid");
		return false;
	}

	WeatherController = NewObject<UWeatherController>(this);
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



void UWeatherSubsystem::HandleWorldBeginTearDown(UWorld* World)
{
}



bool UWeatherSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UWeatherSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("WeatherSubsystem initialized"));

	FWorldDelegates::OnWorldBeginTearDown.RemoveAll(this);
	FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &UWeatherSubsystem::HandleWorldBeginTearDown);
}

void UWeatherSubsystem::OnWorldComponentsUpdated(UWorld& InWorld)
{
	Super::OnWorldComponentsUpdated(InWorld);

	AEnvironmentWorldSettings* WorldSettings = Cast<AEnvironmentWorldSettings>(InWorld.GetWorldSettings());
	if (!IsValid(WorldSettings))
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentWorldSettings is not valid"));
		return;
	}

	EnvironmentAsset = WorldSettings->EnvironmentAsset;
	if (!IsValid(EnvironmentAsset))
	{
		LOG_ERROR(LogTemp, TEXT("EnvironmentAsset is not valid"));
		return;
	}

	if (CreateWeatherController())
	{
		CreateWeatherTimer();
		CreateWeatherMaterialCollection();
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

	TimerUtils::ClearTimer(WeatherTimerHandle, this);

	LOG_WARNING(LogTemp, TEXT("WeatherSubsystem deinitialized"));
	Super::Deinitialize();
}

