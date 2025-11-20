// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreMaterial/Public/MaterialSurfaceProperty.h"

#include "RenWeather/Public/WeatherDelegate.h"
#include "RenWeather/Public/WeatherSurfaceEffect.h"
#include "RenWeather/Public/WeatherControllerInterface.h"

// Generated Headers
#include "WeatherController.generated.h"

// Forward Declarations
class UMaterialParameterCollectionInstance;

class UTimer;
class UWeatherAsset;
class UEnvironmentSubsystem;
class UPriorityList;



/**
 * 
 * 
 */
UCLASS()
class UWeatherController : public UObject, public IWeatherControllerInterface
{

	GENERATED_BODY()

public:

	void Initialize();
	void Deinitialize();

	void SetMaterialCollection(UMaterialParameterCollectionInstance* MaterialCollection);

	bool AddWeather(UWeatherAsset* WeatherAsset, int Priority);
	bool RemoveWeather(int Priority);

	void RefreshWeather();

protected:

	FWeatherDelegates Delegates;

	UPROPERTY()
	TObjectPtr<UTimer> Timer;

	UPROPERTY()
	TObjectPtr<UPriorityList> PriorityList;

	UPROPERTY()
	UMaterialParameterCollectionInstance* MPC;

	UPROPERTY()
	TWeakObjectPtr<UEnvironmentSubsystem> EnvironmentSubsystem;

	UPROPERTY()
	TObjectPtr<UWeatherAsset> CurrentWeather;

	TMap<TObjectPtr<UWeatherAsset>, TArray<TPair<FPrimaryAssetId, FGuid>>> LatentCollection;



	void AddEnvironmentProfile(UWeatherAsset* WeatherAsset);
	void RemoveEnvironmentProfile(UWeatherAsset* WeatherAsset);

	// ~ Item Bindings
	void HandleItemChanged(UObject* Item);
	void HandleItemRemoved(UObject* Item, bool bReplaced);
	void HandleItemCleared();
	// ~ End of Item Bindings

	// ~ Timer
	FMaterialSurfaceProperty SourceSurfaceProperty;
	FWeatherSurfaceEffect SourceSurfaceEffect;

	FMaterialSurfaceProperty TargetSurfaceProperty;
	FWeatherSurfaceEffect TargetSurfaceEffect;

	void StartTransition(UWeatherAsset* WeatherAsset);
	void HandleTimerTick(float ElapsedTime);
	// ~ End of Timer

public:

	// ~ IWeatherControllerInterface
	virtual FWeatherDelegates& GetWeatherDelegates() override;
	// ~ End of IWeatherControllerInterface

};

