// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreCommon/Public/Priority/PrioritySystem.h"
#include "RCoreMaterial/Public/MaterialSurfaceProperty.h"
#include "RenWeather/Public/WeatherAsset.h"

#include "RenWeather/Public/WeatherDelegate.h"

// Generated Headers
#include "WeatherController.generated.h"

// Forward Declarations
class UMaterialParameterCollectionInstance;
class UWeatherAsset;

struct FMaterialSurfaceProperty;
struct FWeatherSurfaceEffect;


/**
 *
 *
 */
UCLASS()
class UTimer : public UObject
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE_TwoParams(FTimerUpdate, float, float);
	FTimerUpdate OnTimerUpdate;

	bool IsTimerValid();


	void SetTimer(float InRate, float InDuration);

	void RestartTimer();
	void ResumeTimer();
	void PauseTimer();
	void ClearTimer();

protected:

	float Rate = 0.0f;
	float Duration = 0.0f;
	float ElapsedTime = 0.0f;

	void TimerTick();

private:

	FTimerHandle TimerHandle;

};


/**
 * 
 * 
 */
UCLASS()
class UWeatherController : public UObjectPrioritySystem
{

	GENERATED_BODY()

public:

	FWeatherDelegates Delegates;

	void SetMaterialCollection(UMaterialParameterCollectionInstance* MaterialCollection);

protected:

	UPROPERTY()
	TObjectPtr<UTimer> WeatherTimer;

	FMaterialSurfaceProperty CurrentSurfaceProperty;
	FWeatherSurfaceEffect CurrentSurfaceEffect;

	FMaterialSurfaceProperty TargetSurfaceProperty;
	FWeatherSurfaceEffect TargetSurfaceEffect;

	TMap<TObjectPtr<UWeatherAsset>, TArray<TPair<FPrimaryAssetId, FGuid>>> LatentCollection;
	TObjectPtr<UWeatherAsset> CurrentWeather;
	UMaterialParameterCollectionInstance* MPC;

	virtual void TimerUpdate(float Rate, float Duration);

	virtual void AddEnvironmentProfile(UWeatherAsset* WeatherAsset);
	virtual void RemoveEnvironmentProfile(UWeatherAsset* WeatherAsset);

	virtual void HandleTransition(const FMaterialSurfaceProperty& SurfaceProperty, const FWeatherSurfaceEffect& SurfaceEffects);

public:

	// ~ UObjectPrioritySystem
	virtual void Initialize() override;
	virtual void Deinitialize() override;
	// ~ End of UObjectPrioritySystem

protected:
	
	// ~ UObjectPrioritySystem
	virtual void HandleItemChanged(UObject* Item) override;
	virtual void HandleItemRemoved(UObject* Item, bool bWasReplaced) override;
	virtual void HandleNoItemsLeft() override;
	// ~ End of UObjectPrioritySystem

};

