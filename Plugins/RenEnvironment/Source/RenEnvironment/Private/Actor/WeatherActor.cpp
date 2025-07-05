// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/WeatherActor.h"

// Engine Headers
#include "InstancedStruct.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Header
#include "RenCore/Public/Priority/PrioritySystem.h"
#include "RenCore/Public/Timer/Timer.h"

#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"

#include "RenEnvironment/Public/Asset/WeatherAsset.h"
#include "RenEnvironment/Public/Controller/WeatherController.h"
#include "RenEnvironment/Public/Subsystem/WeatherSubsystem.h"



void AWeatherActor::AddWeather()
{
	if (!IsValid(WeatherSubsystem) || !IsValid(WeatherAsset))
	{
        LOG_ERROR(LogTemp, "WeatherSubsystem or WeatherAsset is not valid.");
		return;
	}

    WeatherSubsystem->AddWeather(WeatherAsset, Priority);
}

void AWeatherActor::RemoveWeather()
{
	if (!IsValid(WeatherSubsystem))
	{
		LOG_ERROR(LogTemp, "WeatherSubsystem is not valid.");
		return;
	}

    WeatherSubsystem->RemoveWeather(Priority);
}



FString AWeatherActor::Roll(const TMap<int, FString>& Items)
{
    TArray<TPair<int32, FString>> Entries;
    int32 TotalWeight = 0;

    for (const TPair<int32, FString>& Pair : Items)
    {
        Entries.Add(Pair);
        TotalWeight += Pair.Key;
    }

    if (TotalWeight == 0 || Entries.Num() == 0)
    {
        return TEXT("");
    }

    int32 RandomWeight = FMath::RandRange(1, TotalWeight);
    int32 Cumulative = 0;

    for (const TPair<int32, FString>& Entry : Entries)
    {
        Cumulative += Entry.Key;
        if (RandomWeight <= Cumulative)
        {
            return Entry.Value;
        }
    }

    return TEXT("");
}

void AWeatherActor::HandleWeatherCanChange()
{

}


void AWeatherActor::BeginPlay()
{
	Super::BeginPlay();

	WeatherSubsystem = GetWorld()->GetSubsystem<UWeatherSubsystem>();
	if (IsValid(WeatherSubsystem))
	{
		WeatherSubsystem->OnWeatherCanChange.AddDynamic(this, &AWeatherActor::HandleWeatherCanChange);
	}
}

void AWeatherActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(WeatherSubsystem))
	{
		WeatherSubsystem->OnWeatherCanChange.RemoveAll(this);
	}
    WeatherSubsystem = nullptr;

	Super::EndPlay(EndPlayReason);
}

