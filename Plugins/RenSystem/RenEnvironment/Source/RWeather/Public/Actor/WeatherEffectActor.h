// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/Actor.h"

// Generated Headers
#include "WeatherEffectActor.generated.h"


/**
 *
 */
UCLASS(Abstract)
class AWeatherEffectActor : public AActor
{

    GENERATED_BODY()

public:

	AWeatherEffectActor();

    UFUNCTION(BlueprintImplementableEvent)
    void ActivateEffect();

    UFUNCTION(BlueprintImplementableEvent)
    void DeactivateEffect();

};

