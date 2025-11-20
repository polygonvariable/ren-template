// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/WeatherRegionActorBase.h"

// Engine Headers

// Project Header
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreLibrary/Public/LogCategory.h"

#include "RenWeather/Public/WeatherAsset.h"
#include "RenWeather/Public/WeatherSubsystem.h"



void AWeatherRegionActorBase::AddWeather()
{
    UWeatherSubsystem* Weather = WeatherSubsystem.Get();
    if (!IsValid(Weather) || !UWeatherAsset::IsValid(WeatherId))
    {
        PRINT_ERROR(LogWeather, 1.0f, TEXT("WeatherSubsystem, WeatherId is invalid"));
        return;
    }

    //WeatherSubsystem->AddWeather(WeatherId, WeatherPriority);
}

void AWeatherRegionActorBase::RemoveWeather()
{
    UWeatherSubsystem* Weather = WeatherSubsystem.Get();
    if (!IsValid(Weather))
    {
        PRINT_ERROR(LogWeather, 1.0f, TEXT("WeatherSubsystem is invalid"));
        return;
    }

    //WeatherSubsystem->RemoveWeather(WeatherId, WeatherPriority);
}

void AWeatherRegionActorBase::HandleWeatherRefreshed()
{
}

void AWeatherRegionActorBase::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (DoesCollidedWithPlayer(OtherActor) && !bPlayerInRegion)
    {
        bPlayerInRegion = true;
        AddWeather();
    }
}

void AWeatherRegionActorBase::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
    if (DoesCollidedWithPlayer(OtherActor) && bPlayerInRegion)
    {
        bPlayerInRegion = false;
        RemoveWeather();
    }
}

void AWeatherRegionActorBase::BeginPlay()
{
    UWeatherSubsystem* Weather = GetWorld()->GetSubsystem<UWeatherSubsystem>();
    if (IsValid(Weather))
    {
        //Weather->Delegates.OnRefreshed.AddUObject(this, &AWeatherRegionActorBase::HandleWeatherRefreshed);
        WeatherSubsystem = Weather;
    }

    Super::BeginPlay();
}

void AWeatherRegionActorBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    RemoveWeather();

    UWeatherSubsystem* Weather = WeatherSubsystem.Get();
    if (IsValid(Weather))
    {
        //Weather->Delegates.RemoveAll(this);
    }
    WeatherSubsystem.Reset();

    Super::EndPlay(EndPlayReason);
}

