// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/WeatherDelegate.h"
#include "MaterialSurfaceProperty.h"
#include "Priority/PriorityListInterface.h"

// Generated Headers
#include "WeatherController.generated.h"

// Module Macros
#define REN_API RWEATHER_API

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
	REN_API float GetEditorWeatherTransition() const;
	REN_API int GetEditorCurrentWeatherPriority() const;
	REN_API const FString GetEditorCurrentWeatherName() const;
	REN_API const UWeatherAsset* GetEditorCurrentWeather() const;
	REN_API const TMap<int, TWeakObjectPtr<UObject>>& GetEditorWeathers() const;
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
	virtual int& GetHighestPriority() override final;
	virtual void OnPriorityItemChanged(UObject* Item) override;
	virtual void OnPriorityItemRemoved(UObject* Item, bool bReplaced) override;
	virtual void OnPriorityItemCleared() override;
	// ~ End of IPriorityListInterface

private:

	UPROPERTY()
	TMap<int, TWeakObjectPtr<UObject>> _PriorityItems;
	int _HighestPriority = 0;

	float _TransitionRate = 0.5f;
	float _TransitionDuration = 1.0f;
	float _ElapsedTime = 0.0f;

	// ~ Binding
	void HandleOnTransitionTick();
	// ~ End of Binding

};


// Module Macros
#undef REN_API

