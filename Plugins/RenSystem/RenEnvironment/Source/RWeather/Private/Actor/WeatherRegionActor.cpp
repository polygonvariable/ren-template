// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/WeatherRegionActor.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Header
#include "Core/AssetManagerUtil.h"
#include "Core/EnvironmentSettings.h"
#include "Data/WeatherCollectionAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/WeatherSubsystem.h"


void AWeatherRegionActor::LoadWeather()
{
    UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
    if (!IsValid(AssetManager))
    {
        return;
    }

    FAssetManagerUtil::CancelHandle(AssetHandle);

    const UEnvironmentSettings* Settings = UEnvironmentSettings::Get();
    const TArray<FName>& Bundle = Settings->EnvironmentBundles;

    AssetHandle = AssetManager->LoadPrimaryAsset(WeatherCollectiontId, Bundle, FStreamableDelegate::CreateUObject(this, &AWeatherRegionActor::HandleOnWeatherLoaded));
}

void AWeatherRegionActor::AddWeather()
{
    const UWeatherCollectionAsset* WeatherWeight = GetWeatherWeight();
    if (!IsValid(WeatherSubsystem) || !IsValid(WeatherWeight))
    {
        LOG_ERROR(LogWeather, TEXT("WeatherSubsystem, WeatherWeight is invalid"));
        return;
    }

    WeatherSubsystem->AddWeather(WeatherWeight->GetRandomWeather(), WeatherWeight->Priority);
}

void AWeatherRegionActor::RemoveWeather()
{
    FAssetManagerUtil::CancelHandle(AssetHandle);

    const UWeatherCollectionAsset* WeatherWeight = GetWeatherWeight();
    if (!IsValid(WeatherSubsystem) || !IsValid(WeatherWeight))
    {
        LOG_ERROR(LogWeather, TEXT("WeatherSubsystem, WeatherId is invalid"));
        return;
    }

    WeatherSubsystem->RemoveWeather(WeatherWeight->Priority);
}

const UWeatherCollectionAsset* AWeatherRegionActor::GetWeatherWeight() const
{
    UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
    if (!IsValid(AssetManager))
    {
        return nullptr;
    }
    return AssetManager->GetPrimaryAssetObject<UWeatherCollectionAsset>(WeatherCollectiontId);
}


void AWeatherRegionActor::HandleOnWeatherRefreshed()
{
    if (bPlayerInRegion)
    {
        LoadWeather();
    }
}

void AWeatherRegionActor::HandleOnWeatherLoaded()
{
    FAssetManagerUtil::CancelHandle(AssetHandle);
    AddWeather();
}

void AWeatherRegionActor::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (DoesCollidedWithPlayer(OtherActor) && !bPlayerInRegion)
    {
        bPlayerInRegion = true;
        LoadWeather();
    }
}

void AWeatherRegionActor::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
    if (DoesCollidedWithPlayer(OtherActor) && bPlayerInRegion)
    {
        bPlayerInRegion = false;
        RemoveWeather();
    }
}


void AWeatherRegionActor::BeginPlay()
{
    WeatherSubsystem = GetWorld()->GetSubsystem<UWeatherSubsystem>();
    if (IsValid(WeatherSubsystem))
    {
        WeatherSubsystem->OnWeatherRefreshed.AddUObject(this, &AWeatherRegionActor::HandleOnWeatherRefreshed);
    }

    Super::BeginPlay();
}

void AWeatherRegionActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    WeatherSubsystem = GetWorld()->GetSubsystem<UWeatherSubsystem>();
    if (IsValid(WeatherSubsystem))
    {
        WeatherSubsystem->OnWeatherRefreshed.RemoveAll(this);
    }

    RemoveWeather();

    Super::EndPlay(EndPlayReason);
}

