// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreCommon/Public/Priority/PrioritySystem.h"
#include "RCoreMaterial/Public/MaterialSurfaceProperty.h"

#include "RenWeather/Public/WeatherDelegate.h"
#include "RenWeather/Public/WeatherSurfaceEffect.h"

// Generated Headers
#include "WeatherController.generated.h"

// Forward Declarations
class UMaterialParameterCollectionInstance;
class UWeatherAsset;



/**
 * 
 * 
 */
UCLASS()
class UWeatherController : public UPrioritySystem
{

	GENERATED_BODY()

public:

	FWeatherDelegates Delegates;

	void SetMaterialCollection(UMaterialParameterCollectionInstance* MaterialCollection);

protected:

	TObjectPtr<UWeatherAsset> CurrentWeather;
	UMaterialParameterCollectionInstance* MPC;

	TMap<TObjectPtr<UWeatherAsset>, TArray<TPair<FPrimaryAssetId, FGuid>>> LatentCollection;


	virtual void AddEnvironmentProfile(UWeatherAsset* WeatherAsset);
	virtual void RemoveEnvironmentProfile(UWeatherAsset* WeatherAsset);

	virtual void HandleTransition(const FMaterialSurfaceProperty& SurfaceProperty, const FWeatherSurfaceEffect& SurfaceEffects);

public:

	// ~ UPrioritySystem
	virtual void Initialize() override;
	virtual void Deinitialize() override;
	// ~ End of UPrioritySystem

protected:
	
	// ~ UPrioritySystem
	virtual void OnItemChanged(UObject* Item) override;
	virtual void OnItemRemoved(UObject* Item, bool bWasReplaced) override;
	virtual void OnNoItemsLeft() override;
	// ~ End of UPrioritySystem

};






/**
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
*/

