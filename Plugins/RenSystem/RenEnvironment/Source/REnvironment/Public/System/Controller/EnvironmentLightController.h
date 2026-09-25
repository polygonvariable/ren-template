
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "System/EnvironmentController.h"

// Generated Headers
#include "EnvironmentLightController.generated.h"

// Forward Declarations
class UDirectionalLightComponent;


/**
 *
 */
UCLASS()
class UEnvironmentLightController : public UEnvironmentStackedController
{

	GENERATED_BODY()

public:

	UEnvironmentLightController();

	FName SunTag = TEXT("Environment.Sun");
	FName MoonTag = TEXT("Environment.Moon");


	// ~ UEnvironmentStackedController
	virtual void Initialize(AActor* Actor) override;
	virtual void Deinitialize() override;
	// ~ End of UEnvironmentStackedController

protected:

	TWeakObjectPtr<UDirectionalLightComponent> SunComponent;
	TWeakObjectPtr<UDirectionalLightComponent> MoonComponent;

	float CurrentSunRadius = 0.0f;
	float TargetSunRadius = 0.0f;

	float CurrentSunIntensity = 0.0f;
	float TargetSunIntensity = 0.0f;

	FLinearColor CurrentSunColor = FColor::White;
	FLinearColor TargetSunColor = FColor::White;

	float CurrentMoonRadius = 0.0f;
	float TargetMoonRadius = 0.0f;

	float CurrentMoonIntensity = 0.0f;
	float TargetMoonIntensity = 0.0f;

	FLinearColor CurrentMoonColor = FColor::White;
	FLinearColor TargetMoonColor = FColor::White;


	// ~ UEnvironmentStackedController
	virtual void OnPriorityItemChanged(UObject* PreviousItem, UObject* NewItem) override;
	virtual void OnTransitionChanged(float Alpha) override;
	// ~ End of UEnvironmentStackedController

};

