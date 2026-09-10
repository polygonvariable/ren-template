// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/WeatherEffectActor.h"


AWeatherEffectActor::AWeatherEffectActor()
{
    PrimaryActorTick.bCanEverTick = false;
    SetActorTickEnabled(false);
    SetActorEnableCollision(false);
    SetCanBeDamaged(false);
}

