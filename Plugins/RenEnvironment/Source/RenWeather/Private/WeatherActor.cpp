// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherActor.h"

// Engine Headers

// Project Header
#include "RenCore/Public/Macro/LogMacro.h"

#include "RenWeather/Public/WeatherAsset.h"
#include "RenWeather/Public/WeatherSubsystem.h"



void AWeatherRegionActor::AddWeather()
{
    UWeatherSubsystem* WeatherSubsystemPtr = WeatherSubsystem.Get();
    if (!IsValid(WeatherSubsystemPtr) || !IsValid(CurrentWeatherAsset))
    {
        PRINT_ERROR(LogTemp, 1.0f, TEXT("WeatherSubsystem or CurrentWeatherAsset is not valid"));
		return;
    }

    WeatherSubsystem->AddWeather(CurrentWeatherAsset, WeatherPriority);
}

void AWeatherRegionActor::RemoveWeather()
{
    UWeatherSubsystem* WeatherSubsystemPtr = WeatherSubsystem.Get();
    if (!IsValid(WeatherSubsystemPtr))
    {
        PRINT_ERROR(LogTemp, 1.0f, TEXT("WeatherSubsystem is not valid"));
        return;
    }

    WeatherSubsystem->RemoveWeather(WeatherPriority);
}

void AWeatherRegionActor::HandleWeatherRefreshed()
{
    CurrentIndex = (CurrentIndex == 0) ? 1 : 0;

    if (WeatherAssets.IsValidIndex(CurrentIndex))
    {
        CurrentWeatherAsset = WeatherAssets[CurrentIndex];
        if (bPlayerInRegion)
        {
            AddWeather();
        }
    }
}

void AWeatherRegionActor::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (DoesCollidedWithPlayer(OtherActor))
    {
        Super::HandlePlayerEntered(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
        AddWeather();
    }
}

void AWeatherRegionActor::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
    if (DoesCollidedWithPlayer(OtherActor))
    {
		Super::HandlePlayerExited(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
        RemoveWeather();
    }
}

void AWeatherRegionActor::BeginPlay()
{
    UWeatherSubsystem* WeatherSubsystemPtr = GetWorld()->GetSubsystem<UWeatherSubsystem>();
	if (IsValid(WeatherSubsystemPtr))
	{
        WeatherSubsystemPtr->OnWeatherRefreshed.AddUObject(this, &AWeatherRegionActor::HandleWeatherRefreshed);
        WeatherSubsystem = WeatherSubsystemPtr;
	}

    Super::BeginPlay();
}

void AWeatherRegionActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    RemoveWeather();

    UWeatherSubsystem* WeatherSubsystemPtr = WeatherSubsystem.Get();
    if (IsValid(WeatherSubsystemPtr))
    {
		WeatherSubsystemPtr->OnWeatherRefreshed.RemoveAll(this);
    }
    WeatherSubsystem.Reset();

	Super::EndPlay(EndPlayReason);
}

