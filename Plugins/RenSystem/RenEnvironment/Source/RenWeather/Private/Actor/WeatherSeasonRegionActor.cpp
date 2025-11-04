// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/WeatherSeasonRegionActor.h"

// Engine Headers

// Project Header
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreLibrary/Public/SubsystemUtils.h"

#include "RenEnvironment/Public/Interface/SeasonProviderInterface.h"



void AWeatherSeasonRegionActor::HandleWeatherRefreshed()
{
    ISeasonProviderInterface* SeasonInterfacePtr = SeasonInterface.Get();
    if (!SeasonInterfacePtr)
    {
        PRINT_ERROR(LogTemp, 1.0f, TEXT("SeasonInterface is not valid"));
		return;
    }

    /*UPrimaryDataAsset* CurrentSeason = SeasonInterfacePtr->GetSeasonAsset();
    const FWeatherCollection& ItemCollection = WeatherCollections.FindRef(CurrentSeason);

    CurrentWeather = ItemCollection.GetRandomItem();
    if (bPlayerInRegion)
    {
        AddWeather();
    }*/
}

void AWeatherSeasonRegionActor::BeginPlay()
{
    ISeasonProviderInterface* SeasonInterfacePtr = SubsystemUtils::GetSubsystemInterface<UWorld, UWorldSubsystem, ISeasonProviderInterface>(GetWorld());
    if (SeasonInterfacePtr)
    {
        SeasonInterface = TWeakInterfacePtr<ISeasonProviderInterface>(SeasonInterfacePtr);
    }

	Super::BeginPlay();
}

void AWeatherSeasonRegionActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    SeasonInterface.Reset();

	Super::EndPlay(EndPlayReason);
}

