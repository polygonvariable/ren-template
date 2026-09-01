// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/WeatherSeasonRegionActor.h"

// Engine Headers

// Project Header
#include "Log/LogMacro.h"
#include "Util/SubsystemUtil.h"

#include "Core/SeasonProviderInterface.h"



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
    ISeasonProviderInterface* SeasonInterfacePtr = SubsystemUtil::GetSubsystemInterface<UWorld, UWorldSubsystem, ISeasonProviderInterface>(GetWorld());
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

