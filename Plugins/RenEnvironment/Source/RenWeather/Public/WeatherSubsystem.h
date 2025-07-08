// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers

// Generated Headers
#include "WeatherSubsystem.generated.h"

// Forward Declarations
class UWeatherController;
class UEnvironmentAsset;
class UWeatherAsset;
class AWeatherEffectActor;


/**
 *
 */
UCLASS()
class UWeatherSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	bool AddWeather(UWeatherAsset* WeatherAsset, int Priority);

	UFUNCTION(BlueprintCallable)
	bool RemoveWeather(int Priority);

	UWeatherController* GetWeatherController() const;

protected:

	float WeatherChangeTime = 5.0f;

	UPROPERTY()
	TMap<TSubclassOf<AWeatherEffectActor>, TObjectPtr<AWeatherEffectActor>> EffectActors;

	UPROPERTY()
	FTimerHandle WeatherTimerHandle;

	UPROPERTY()
	TObjectPtr<UWeatherController> WeatherController;

	UPROPERTY()
	TObjectPtr<UEnvironmentAsset> EnvironmentAsset;


	void CreateWeatherTimer();
	bool CreateWeatherController();
	void CreateWeatherMaterialCollection();

	UFUNCTION()
	void HandleWeatherTimer();
	void HandleItemChanged(UWeatherAsset* WeatherAsset);
	void HandleItemRemoved(UWeatherAsset* WeatherAsset);

protected:

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;

public:

	DECLARE_MULTICAST_DELEGATE(FOnWeatherCanChange);
	FOnWeatherCanChange OnWeatherCanChange;

};

