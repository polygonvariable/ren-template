// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/WeatherEffectManagerActor.h"

// Engine Headers

// Project Header
#include "RenCore/Public/Macro/LogMacro.h"

#include "RenWeather/Public/WeatherAsset.h"
#include "RenWeather/Public/WeatherSubsystem.h"
#include "RenWeather/Public/Actor/WeatherEffectActor.h"



AWeatherEffectManagerActor::AWeatherEffectManagerActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AWeatherEffectManagerActor::OnWeatherChanged(UWeatherAsset* WeatherAsset)
{
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParameters.Owner = this;

    UWorld* World = GetWorld();

    const TSet<TSubclassOf<AWeatherEffectActor>>& WeatherEffects = WeatherAsset->WeatherEffects;
    for (auto& WeatherEffect : WeatherEffects)
    {
        AWeatherEffectActor* EffectActor = EffectActors.FindRef(WeatherEffect);
        if (IsValid(EffectActor))
        {
            EffectActor->ActivateEffect();
        }
        else
        {
            AWeatherEffectActor* NewEffectActor = World->SpawnActor<AWeatherEffectActor>(WeatherEffect, SpawnParameters);
            if (IsValid(NewEffectActor))
            {
                NewEffectActor->ActivateEffect();
                EffectActors.Add(WeatherEffect, NewEffectActor);
            }
        }
    }
}

void AWeatherEffectManagerActor::OnWeatherRemoved(UWeatherAsset* WeatherAsset)
{
    const TSet<TSubclassOf<AWeatherEffectActor>>& WeatherEffects = WeatherAsset->WeatherEffects;
    for (auto& WeatherEffect : WeatherEffects)
    {
        AWeatherEffectActor* EffectActor = EffectActors.FindRef(WeatherEffect);
        if (IsValid(EffectActor))
        {
            EffectActor->DeactivateEffect();
        }
    }
}

void AWeatherEffectManagerActor::BeginPlay()
{
    UWeatherSubsystem* WeatherSubsystemPtr = GetWorld()->GetSubsystem<UWeatherSubsystem>();
    if (IsValid(WeatherSubsystemPtr))
    {
        WeatherSubsystemPtr->GetOnWeatherChanged().AddUObject(this, &AWeatherEffectManagerActor::OnWeatherChanged);
		WeatherSubsystemPtr->GetOnWeatherRemoved().AddUObject(this, &AWeatherEffectManagerActor::OnWeatherRemoved);

        WeatherSubsystem = WeatherSubsystemPtr;
    }

    Super::BeginPlay();
}

void AWeatherEffectManagerActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (auto& EffectActor : EffectActors)
    {
        if (IsValid(EffectActor.Value))
        {
            EffectActor.Value->Destroy();
        }
    }
    EffectActors.Empty();

    UWeatherSubsystem* WeatherSubsystemPtr = WeatherSubsystem.Get();
    if (IsValid(WeatherSubsystemPtr))
    {
        WeatherSubsystemPtr->GetOnWeatherChanged().RemoveAll(this);
		WeatherSubsystemPtr->GetOnWeatherRemoved().RemoveAll(this);
    }
    WeatherSubsystem.Reset();

    Super::EndPlay(EndPlayReason);
}

