// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Project Headers

// Generated Headers
#include "WeatherManagerActor.generated.h"

// Forward Declarations
class URAssetManager;

class UWeatherSubsystem;
class UWeatherAsset;
class AWeatherEffectActor;



/**
 * 
 * 
 */
UCLASS(Abstract)
class AWeatherManagerActor : public AActor
{

	GENERATED_BODY()

public:

	AWeatherManagerActor();

protected:

	TMap<UWeatherAsset*, FGuid> LatentIds;

	TObjectPtr<URAssetManager> AssetManager;
	TWeakObjectPtr<UWeatherSubsystem> WeatherSubsystem;

	UPROPERTY()
	TMap<TSubclassOf<AWeatherEffectActor>, TObjectPtr<AWeatherEffectActor>> EffectActors;


	void HandleWeatherChanged(UPrimaryDataAsset* WeatherAsset);
	void HandleWeatherRemoved(UPrimaryDataAsset* WeatherAsset);

	void ActivateEffects(const TArray<UClass*>& Classes);
	void DeactivateEffects(const TArray<TSoftClassPtr<AWeatherEffectActor>>& Classes);

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};

