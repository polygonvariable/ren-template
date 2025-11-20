
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEnvironment/Public/Controller/EnvironmentController.h"

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

protected:

	TWeakObjectPtr<UDirectionalLightComponent> SunComponent;
	TWeakObjectPtr<UDirectionalLightComponent> MoonComponent;

public:

	// ~ UEnvironmentStackedController
	virtual void Initialize(AActor* Actor) override;
	virtual void Deinitialize() override;
	// ~ End of UEnvironmentStackedController

protected:

	float CurrentSunIntensity = 0.0f;
	FColor CurrentSunColor = FColor::White;

	float CurrentMoonIntensity = 0.0f;
	FColor CurrentMoonColor = FColor::White;

	float TargetSunIntensity = 0.0f;
	FColor TargetSunColor = FColor::White;

	float TargetMoonIntensity = 0.0f;
	FColor TargetMoonColor = FColor::White;

	// ~ UEnvironmentStackedController
	virtual void HandleItemChanged(UObject* Item) override;
	virtual void HandleTimerTick(float ElapsedTime) override;
	// ~ End of UEnvironmentStackedController

};

