// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "RenWeather/Public/WeatherProfile.h"

// Generated Headers
#include "WeatherSubsystem.generated.h"

// Forward Declarations
class UWeatherController;
class UEnvironmentAsset;



/**
 *
 */
UCLASS()
class UWeatherSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void AddWeather(UWeatherAsset* WeatherAsset, int Priority);

	UFUNCTION(BlueprintCallable)
	void RemoveWeather(int Priority);

protected:

	float WeatherChangeTime = 10.0f;

	UPROPERTY()
	FTimerHandle WeatherTimerHandle;

	UPROPERTY()
	TObjectPtr<UWeatherController> WeatherController;

	UPROPERTY()
	TObjectPtr<UEnvironmentAsset> EnvironmentAsset;


	UFUNCTION()
	void HandleWeatherTimer();

	void CreateWeatherTimer();
	bool CreateWeatherController();
	void CreateWeatherMaterialCollection();


	void HandleWorldBeginTearDown(UWorld* World);

protected:

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeatherChanged);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnWeatherChanged OnWeatherChanged;

};

