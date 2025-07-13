// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "RenWeather/Public/WeatherDelegate.h"

// Generated Headers
#include "WeatherSubsystem.generated.h"

// Forward Declarations
class UObjectPrioritySystem;
class UEnvironmentAsset;
class UWeatherController;
class UWeatherAsset;



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
	
protected:

	FDelegateHandle OnWeatherChangedHandle;
	FDelegateHandle OnWeatherRemovedHandle;

	UPROPERTY()
	FTimerHandle WeatherTimer;

	UPROPERTY()
	TObjectPtr<UWeatherController> WeatherController;


	bool CreateWeatherTimer(float RefreshTime);
	bool CreateWeatherController(TSubclassOf<UObjectPrioritySystem> ControllerClass);
	void CreateWeatherMaterialCollection(UMaterialParameterCollection* Collection);

	void HandleWeatherTimer();

protected:

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;

public:

	FOnWeatherRefreshed OnWeatherRefreshed;
	FOnWeatherChanged OnWeatherChanged;
	FOnWeatherRemoved OnWeatherRemoved;

};

