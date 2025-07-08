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
        PRINT_ERROR(LogTemp, 1.0f, TEXT("WeatherSubsystem or WeatherAsset is not valid"));
        return;
    }

    WeatherSubsystem->RemoveWeather(WeatherPriority);
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

void AWeatherRegionActor::HandleWeatherCanChanged()
{
    int Length = WeatherAssets.Num();
    int RandomIndex = FMath::RandRange(0, Length - 1);

    if (WeatherAssets.IsValidIndex(RandomIndex))
    {
		CurrentWeatherAsset = WeatherAssets[RandomIndex];
        if (bPlayerInRegion)
        {
            // RemoveWeather();
            AddWeather();
        }
    }
}


void AWeatherRegionActor::BeginPlay()
{
    UWeatherSubsystem* WeatherSubsystemPtr = GetWorld()->GetSubsystem<UWeatherSubsystem>();
	if (IsValid(WeatherSubsystemPtr))
	{
        WeatherSubsystemPtr->OnWeatherCanChange.AddUObject(this, &AWeatherRegionActor::HandleWeatherCanChanged);
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
		WeatherSubsystemPtr->OnWeatherCanChange.RemoveAll(this);
    }
    WeatherSubsystem.Reset();

	Super::EndPlay(EndPlayReason);
}

