// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/WeatherRegionActorBase.h"

// Engine Headers

// Project Header
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenWeather/Public/WeatherAsset.h"
#include "RenWeather/Public/WeatherSubsystem.h"



void AWeatherRegionActorBase::AddWeather()
{
    UWeatherSubsystem* WeatherSubsystemPtr = WeatherSubsystem.Get();
    if (!IsValid(WeatherSubsystemPtr) || !IsValid(CurrentWeather))
    {
        PRINT_ERROR(LogTemp, 1.0f, TEXT("WeatherSubsystem or CurrentWeather is not valid"));
        return;
    }

    WeatherSubsystem->AddWeather(CurrentWeather, WeatherPriority);
}

void AWeatherRegionActorBase::RemoveWeather()
{
    UWeatherSubsystem* WeatherSubsystemPtr = WeatherSubsystem.Get();
    if (!IsValid(WeatherSubsystemPtr))
    {
        PRINT_ERROR(LogTemp, 1.0f, TEXT("WeatherSubsystem is not valid"));
        return;
    }

    WeatherSubsystem->RemoveWeather(WeatherPriority);
}

void AWeatherRegionActorBase::HandleWeatherRefreshed()
{
}

void AWeatherRegionActorBase::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (DoesCollidedWithPlayer(OtherActor))
    {
        Super::HandlePlayerEntered(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
        AddWeather();
    }
}

void AWeatherRegionActorBase::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
    if (DoesCollidedWithPlayer(OtherActor))
    {
        Super::HandlePlayerExited(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
        RemoveWeather();
    }
}

void AWeatherRegionActorBase::BeginPlay()
{
    UWeatherSubsystem* WeatherSubsystemPtr = GetWorld()->GetSubsystem<UWeatherSubsystem>();
    if (IsValid(WeatherSubsystemPtr))
    {
        WeatherSubsystemPtr->OnWeatherRefreshed.AddUObject(this, &AWeatherRegionActorBase::HandleWeatherRefreshed);
        WeatherSubsystem = WeatherSubsystemPtr;
    }

    Super::BeginPlay();
}

void AWeatherRegionActorBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
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

