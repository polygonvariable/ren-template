// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "Core/WeatherDelegate.h"

// Generated Headers
#include "WeatherSubsystem.generated.h"

// Forward Declarations
class UAssetManager;
class UWeatherController;
class UWeatherAsset;
class UWeatherWorldConfig;
struct FStreamableHandle;


/**
 * 
 */
UCLASS(MinimalAPI)
class UWeatherSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE(FOnWeatherControllerCreated);
	FOnWeatherControllerCreated OnControllerCreated;

	FOnWeatherRefreshed OnWeatherRefreshed;


	bool AddWeather(UWeatherAsset* WeatherAsset, int Priority);
	bool RemoveWeather(int Priority);

	RWEATHER_API UWeatherController* GetWeatherController() const;

	// ~ UWorldSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void OnWorldEndPlay(UWorld& InWorld) override;
	// ~ End of UWorldSubsystem

protected:

	UPROPERTY()
	TObjectPtr<const UWeatherWorldConfig> WeatherConfig;

	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager;

	UPROPERTY()
	TObjectPtr<AActor> EffectManager;

	UPROPERTY()
	TObjectPtr<UWeatherController> WeatherController;

	TSharedPtr<FStreamableHandle> WeatherHandle;
	FTimerHandle WeatherTimer;
	

	bool CreateWeatherTimer(float RefreshTime);
	void RemoveWeatherTimer();

	bool CreateWeatherController(UClass* ControllerClass, UMaterialParameterCollection* MPC);
	void RemoveWeatherController();

	bool CreateWeatherManager(UClass* ManagerClass);
	void RemoveWeatherManager();

	void RegisterDefaultWeather(const FPrimaryAssetId& AssetId, int Priority);
	
	// ~ Binding
	void HandleOnWeatherTimerTick();
	void HandleOnWeatherLoaded();
	// ~ End of Binding

	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	// ~ End of UWorldSubsystem

public:

	static RWEATHER_API UWeatherSubsystem* Get(UWorld* World);

};

