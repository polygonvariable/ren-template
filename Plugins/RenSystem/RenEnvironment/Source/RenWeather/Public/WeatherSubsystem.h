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
class URAssetManager;

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

	FWeatherDelegates Delegates;

	bool AddWeather(UWeatherAsset* WeatherAsset, int Priority);
	bool RemoveWeather(int Priority);

	UFUNCTION(BlueprintCallable)
	void AddWeather(const FGuid& LatentId, const FPrimaryAssetId& AssetId, int Priority);

	UFUNCTION(BlueprintCallable)
	bool RemoveWeather(const FGuid& LatentId, int Priority);

protected:

	FTimerHandle WeatherTimer;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager;

	UPROPERTY()
	TObjectPtr<AActor> WeatherManager;

	UPROPERTY()
	TObjectPtr<UWeatherController> WeatherController;
	

	void LoadWeatherManager(const FSoftClassPath& ClassPath);
	void LoadDefaultWeather(const FPrimaryAssetId& AssetId);

	bool CreateWeatherTimer(float RefreshTime);
	bool CreateWeatherController(TSubclassOf<UObjectPrioritySystem> ControllerClass);
	bool CreateWeatherMPC(UMaterialParameterCollection* Collection);

	// ~ Bindings
	void HandleWeatherTimer();
	// ~ End of Bindings

protected:

	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

};

