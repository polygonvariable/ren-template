// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/WeatherDelegate.h"
#include "MaterialSurfaceProperty.h"
#include "Priority/PriorityListInterface.h"

// Generated Headers
#include "WeatherController.generated.h"

// Forward Declarations
class UMaterialParameterCollectionInstance;
class UWeatherAsset;
class UEnvironmentSubsystem;


/**
 * 
 */
UCLASS(MinimalAPI)
class UWeatherController : public UObject, public IPriorityListInterface
{

	GENERATED_BODY()

public:

	FOnWeatherChanged OnWeatherChanged;
	FOnWeatherChanged OnWeatherRemoved;


	void Initialize(UMaterialParameterCollectionInstance* InMPCInstance);
	void Deinitialize();

	bool AddWeather(UWeatherAsset* WeatherAsset, int Priority);
	bool RemoveWeather(int Priority);

#if UE_BUILD_DEVELOPMENT
	RWEATHER_API float GetEditorWeatherTransition() const;
	RWEATHER_API int GetEditorCurrentWeatherPriority() const;
	RWEATHER_API const FString GetEditorCurrentWeatherName() const;
	RWEATHER_API const UWeatherAsset* GetEditorCurrentWeather() const;
	RWEATHER_API const TMap<int, TWeakObjectPtr<UObject>>& GetEditorWeathers() const;
#endif

protected:

	UPROPERTY()
	TObjectPtr<UMaterialParameterCollectionInstance>  MPCInstance;

	UPROPERTY()
	TObjectPtr<UEnvironmentSubsystem> EnvironmentSubsystem;

	UPROPERTY()
	TObjectPtr<UWeatherAsset> CurrentWeather;

	UPROPERTY()
	TObjectPtr<UCurveFloat> TransitionCurve = nullptr;

	FTimerHandle TimerHandle;
	FMaterialSurfaceProperty SourceSurfaceProperty;
	FMaterialSurfaceProperty TargetSurfaceProperty;


	void AddEnvironmentProfile();
	void RemoveEnvironmentProfile(UWeatherAsset* WeatherAsset);

	void StartTransition();
	void ClearTransition();
	void OnTransitionChanged(float Alpha);

	// ~ IPriorityListInterface
	virtual TMap<int, TWeakObjectPtr<UObject>>& GetPriorityItems() override final;
	virtual void OnPriorityItemChanged(UObject* PreviousItem, UObject* NewItem) override;
	virtual void OnPriorityItemRemoved(UObject* Item, bool bReplaced) override;
	virtual void OnPriorityItemCleared() override;
	// ~ End of IPriorityListInterface

private:

	UPROPERTY()
	TMap<int, TWeakObjectPtr<UObject>> _WeatherItems;

	float _TransitionRate = 0.5f;
	float _TransitionDuration = 1.0f;
	float _ElapsedTime = 0.0f;

	// ~ Binding
	void HandleOnTransitionTick();
	// ~ End of Binding

};

