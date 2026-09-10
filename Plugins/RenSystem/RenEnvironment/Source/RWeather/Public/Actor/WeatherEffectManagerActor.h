// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "WeatherEffectManagerActor.generated.h"

// Forward Declarations
class UWeatherAsset;
class UWeatherSubsystem;
class AWeatherEffectActor;
struct FStreamableHandle;


/**
 * 
 */
UCLASS(NotPlaceable)
class AWeatherEffectManagerActor : public AInfo
{

	GENERATED_BODY()

public:

	AWeatherEffectManagerActor();

protected:

	UPROPERTY()
	TObjectPtr<UWeatherSubsystem> WeatherSubsystem;

	UPROPERTY()
	TArray<TObjectPtr<AWeatherEffectActor>> EffectActors;

	TMap<TObjectPtr<UWeatherAsset>, TSharedPtr<FStreamableHandle>> LoadHandles;


	void ActivateEffects(const TArray<TSoftClassPtr<AWeatherEffectActor>>& Classes);
	void DeactivateEffects(const TArray<TSoftClassPtr<AWeatherEffectActor>>& Classes);

	void RemoveLoadHandle(UWeatherAsset* WeatherAsset);

	// ~ Binding
	void HandleOnEffectLoaded(UWeatherAsset* WeatherAsset);
	void HandleWeatherChanged(UWeatherAsset* WeatherAsset);
	void HandleWeatherRemoved(UWeatherAsset* WeatherAsset);
	// ~ End of Binding

	// ~ AActor
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of AActor

};

