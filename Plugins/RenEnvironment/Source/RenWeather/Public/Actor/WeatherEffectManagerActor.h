// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Project Headers

// Generated Headers
#include "WeatherEffectManagerActor.generated.h"

// Forward Declarations
class UWeatherSubsystem;
class UWeatherAsset;
class AWeatherEffectActor;



/**
 *
 */
UCLASS(Abstract)
class AWeatherEffectManagerActor : public AActor
{

	GENERATED_BODY()

public:

	AWeatherEffectManagerActor();

protected:

	UPROPERTY()
	TWeakObjectPtr<UWeatherSubsystem> WeatherSubsystem;

	UPROPERTY()
	TMap<TSubclassOf<AWeatherEffectActor>, TObjectPtr<AWeatherEffectActor>> EffectActors;


	void OnWeatherChanged(UWeatherAsset* WeatherAsset);
	void OnWeatherRemoved(UWeatherAsset* WeatherAsset);

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

