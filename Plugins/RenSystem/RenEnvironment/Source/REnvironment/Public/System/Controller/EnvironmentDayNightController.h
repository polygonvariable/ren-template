
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "System/EnvironmentController.h"

// Generated Headers
#include "EnvironmentDayNightController.generated.h"

// Forward Declarations
class UOrbitalLightComponent;
class IClockManagerInterface;


/**
 *
 */
UCLASS()
class UEnvironmentDayNightController : public UEnvironmentDiscreteController
{

	GENERATED_BODY()

public:

	// ~ UEnvironmentDiscreteController
	virtual void Initialize(AActor* Actor) override;
	virtual void Deinitialize() override;
	// ~ UEnvironmentDiscreteController

protected:

	UPROPERTY()
	TWeakObjectPtr<UOrbitalLightComponent> SunComponent;

	UPROPERTY()
	TWeakObjectPtr<UOrbitalLightComponent> MoonComponent;

	IClockManagerInterface* ClockManager;


	// ~ Binding
	void HandleOnTimeChanged(int Time);
	// ~ End of Binding

};

