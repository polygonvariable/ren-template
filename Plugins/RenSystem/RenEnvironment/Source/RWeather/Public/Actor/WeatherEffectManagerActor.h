// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "WeatherEffectManagerActor.generated.h"

// Module Macros
#define REN_API RWEATHER_API

// Forward Declarations
class UWeatherAsset;
class UWeatherSubsystem;
class AWeatherEffectActor;
class UNiagaraSystem;
struct FStreamableHandle;


/**
 * 
 */
UCLASS(MinimalAPI, NotPlaceable)
class AWeatherEffectManagerActor : public AInfo
{

	GENERATED_BODY()

public:

	AWeatherEffectManagerActor();

#if UE_BUILD_DEVELOPMENT
	REN_API const TMap<TObjectPtr<UWeatherAsset>, TSharedPtr<FStreamableHandle>>& GetEditorWeatherEffectHandles() const;
	REN_API const TArray<TObjectPtr<AWeatherEffectActor>>& GetEditorWeatherEffects() const;
#endif

protected:

	UPROPERTY()
	TObjectPtr<UWeatherSubsystem> WeatherSubsystem;

	UPROPERTY()
	TArray<TObjectPtr<AWeatherEffectActor>> EffectActors;

	TMap<TObjectPtr<UWeatherAsset>, TSharedPtr<FStreamableHandle>> LoadHandles;


	void ActivateEffects(const TArray<TSoftObjectPtr<UNiagaraSystem>>& Systems);
	void DeactivateEffects(const TArray<TSoftObjectPtr<UNiagaraSystem>>& Systems);
	void DeactivateAllEffects();

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


// Module Macros
#undef REN_API

