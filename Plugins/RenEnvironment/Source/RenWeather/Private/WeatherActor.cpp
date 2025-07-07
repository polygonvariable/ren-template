// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WeatherActor.h"

// Engine Headers
#include "GameFramework/Character.h"
#include "InstancedStruct.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Project Header
#include "RenCore/Public/Priority/PrioritySystem.h"
#include "RenCore/Public/Timer/Timer.h"

#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"

#include "RenEnvironment/Public/Asset/WeatherAsset.h"
#include "RenWeather/Public/WeatherController.h"
#include "RenWeather/Public/WeatherSubsystem.h"


void AWeatherRegionActor::AddWeather()
{
    UWeatherSubsystem* WeatherSubsystemPtr = WeatherSubsystem.Get();
    if (!IsValid(WeatherSubsystemPtr) || !IsValid(WeatherAsset))
    {
        PRINT_ERROR(LogTemp, 1.0f, TEXT("WeatherSubsystem or WeatherAsset is not valid"));
		return;
    }

    WeatherSubsystem->AddWeather(WeatherAsset, Priority);
}

void AWeatherRegionActor::RemoveWeather()
{
    UWeatherSubsystem* WeatherSubsystemPtr = WeatherSubsystem.Get();
    if (!IsValid(WeatherSubsystemPtr) || !IsValid(WeatherAsset))
    {
        PRINT_ERROR(LogTemp, 1.0f, TEXT("WeatherSubsystem or WeatherAsset is not valid"));
        return;
    }

    WeatherSubsystem->RemoveWeather(Priority);
}


void AWeatherRegionActor::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (DoesCollidedWithPlayer(OtherActor))
    {
        AddWeather();
    }
}

void AWeatherRegionActor::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
    if (DoesCollidedWithPlayer(OtherActor))
    {
        RemoveWeather();
    }
}


void AWeatherRegionActor::HandleWeatherCanChange()
{
    
}


void AWeatherRegionActor::BeginPlay()
{
    UWeatherSubsystem* WeatherSubsystemPtr = GetWorld()->GetSubsystem<UWeatherSubsystem>();
	if (IsValid(WeatherSubsystemPtr))
	{
        WeatherSubsystemPtr->OnWeatherChanged.AddDynamic(this, &AWeatherRegionActor::HandleWeatherCanChange);
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
		WeatherSubsystemPtr->OnWeatherChanged.RemoveAll(this);
    }
    WeatherSubsystem.Reset();

	Super::EndPlay(EndPlayReason);
}
