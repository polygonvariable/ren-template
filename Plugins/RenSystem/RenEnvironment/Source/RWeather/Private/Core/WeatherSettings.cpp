// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/WeatherSettings.h"

// Engine Headers
#include "NiagaraComponent.h"

// Project Headers
#include "Actor/WeatherEffectActor.h"
#include "System/WeatherSubsystem.h"


UWeatherSettings::UWeatherSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	SubsystemClass = UWeatherSubsystem::StaticClass();
	
	EffectActors.Add(USoundBase::StaticClass(), AWeatherEffectAudio::StaticClass());
	EffectActors.Add(UNiagaraSystem::StaticClass(), AWeatherEffectParticle::StaticClass());
}

const UWeatherSettings* UWeatherSettings::Get()
{
	return GetDefault<UWeatherSettings>();
}

