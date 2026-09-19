// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "WeatherEffectManager.generated.h"

// Forward Declarations
class UWeatherAsset;
class UWeatherSubsystem;
class AWeatherEffectActor;
class UNiagaraSystem;
struct FStreamableHandle;


/**
 * 
 */
UCLASS(MinimalAPI, NotBlueprintable, NotPlaceable)
class AWeatherEffectManager : public AInfo
{

	GENERATED_BODY()

public:

	AWeatherEffectManager();

#if UE_BUILD_DEVELOPMENT
	RWEATHER_API const TMap<TObjectPtr<UWeatherAsset>, TSharedPtr<FStreamableHandle>>& GetEditorWeatherEffectHandles() const;
	RWEATHER_API const TArray<TObjectPtr<AWeatherEffectActor>>& GetEditorWeatherEffects() const;
#endif

protected:

	UPROPERTY()
	TObjectPtr<UWeatherSubsystem> WeatherSubsystem;

	UPROPERTY()
	TArray<TObjectPtr<AWeatherEffectActor>> EffectActors;

	TMap<TObjectPtr<UWeatherAsset>, TSharedPtr<FStreamableHandle>> LoadHandles;


	UClass* GetEffectActorClass(UObject* Object) const;

	void ActivateEffects(const TArray<FSoftObjectPath>& Effects);
	void DeactivateEffects(const TArray<FSoftObjectPath>& Effects);
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

