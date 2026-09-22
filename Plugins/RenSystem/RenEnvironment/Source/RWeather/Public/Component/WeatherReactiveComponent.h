// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Generated Headers
#include "WeatherReactiveComponent.generated.h"

// Forward Declarations
class UWeatherSubsystem;
class UWeatherController;
class UWeatherAsset;
class FObjectPreSaveContext;


/*
 *
 */
struct FWeatherEffectTransitionHandle
{
	int EffectIndex = INDEX_NONE;
	TWeakObjectPtr<UActorComponent> TargetComponent;
	float Elapsed = 0.0f;
	float Duration = 1.0f;
	float StartAlpha = 0.0f;
	float TargetAlpha = 0.0f;
};


/**
 * 
 */
UCLASS(ClassGroup = (Custom), editinlinenew, meta = (BlueprintSpawnableComponent))
class UWeatherReactiveComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UWeatherReactiveComponent();

	// ~ UActorComponent
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
	// ~ End of UActorComponent

#if WITH_EDITOR
	// ~ UActorComponent
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UActorComponent
#endif

protected:

	UPROPERTY(EditAnywhere, meta=(BaseStruct="/Script/RWeather.WeatherReactiveEffect", ExcludeBaseStruct))
	TArray<FInstancedStruct> WorldEffects;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1"))
	float TransitionRate = 0.5f;

	UPROPERTY()
	TObjectPtr<UWeatherController> WeatherController;

	UPROPERTY()
	TObjectPtr<UWeatherSubsystem> WeatherSubsystem;


	// ~ Binding
	void HandleOnWeatherChanged(UWeatherAsset* WeatherAsset);
	void HandleOnWeatherRemoved(UWeatherAsset* WeatherAsset);
	void HandleOnControllerCreated();
	// ~ Binding

	void StartTransition(int EffectIndex, UActorComponent* Component, bool bIsAdded);
	void StopTransition(int EffectIndex);
	void CancelAllTransition();

	// ~ Binding
	void HandleOnTransitionTick();
	// ~ Binding

private:

	TArray<FWeatherEffectTransitionHandle> ActiveTransitions;
	FTimerHandle TransitionTimer;

};

