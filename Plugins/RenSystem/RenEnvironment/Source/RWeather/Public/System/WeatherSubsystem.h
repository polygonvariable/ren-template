// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "Core/WeatherDelegate.h"

// Generated Headers
#include "WeatherSubsystem.generated.h"

// Module Macros
#define REN_API RWEATHER_API

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

	FOnWeatherRefreshed OnWeatherRefreshed;


	bool AddWeather(UWeatherAsset* WeatherAsset, int Priority);
	bool RemoveWeather(int Priority);

	REN_API UWeatherController* GetWeatherController() const;

protected:

	UPROPERTY()
	TObjectPtr<AActor> EffectManager;

	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager;

	UPROPERTY()
	TObjectPtr<UWeatherController> WeatherController;

	TSharedPtr<FStreamableHandle> WeatherHandle;
	FTimerHandle WeatherTimer;
	

	bool CreateWeatherTimer(float RefreshTime);
	void RemoveWeatherTimer();

	bool CreateWeatherController(UClass* ControllerClass, UMaterialParameterCollection* MPC);
	void RemoveWeatherController();

	void RegisterDefaultWeather(const FPrimaryAssetId& AssetId, int Priority);
	
	const UWeatherWorldConfig* GetWeatherWorldConfig() const;

	// ~ Bindings
	void HandleOnWeatherTimerTick();
	void HandleOnWeatherLoaded();
	// ~ End of Bindings

protected:

	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void OnWorldEndPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

public:

	static REN_API UWeatherSubsystem* Get(UWorld* World);

};


// Module Macros
#undef REN_API

