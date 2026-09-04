
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "System/EnvironmentController.h"

// Generated Headers
#include "EnvironmentDayNightController.generated.h"

// Forward Declarations
class UOrbitalLightComponent;
class IClockProviderInterface;
class IClockManagerInterface;


/**
 *
 */
UCLASS()
class UEnvironmentDayNightController : public UEnvironmentDiscreteController
{

	GENERATED_BODY()

public:

	FName SunComponentTag = TEXT("Environment.Sun");
	FName MoonComponentTag = TEXT("Environment.Moon");


	// ~ UEnvironmentDiscreteController
	virtual void Initialize(AActor* Actor) override;
	virtual void Deinitialize() override;
	// ~ UEnvironmentDiscreteController

protected:

	UPROPERTY()
	TWeakObjectPtr<UOrbitalLightComponent> SunComponent;

	UPROPERTY()
	TWeakObjectPtr<UOrbitalLightComponent> MoonComponent;

	TWeakInterfacePtr<IClockManagerInterface> ClockManagerInterface;
	FTimerHandle DayTimerHandle;


	void StartDayTimer();
	void StopDayTimer();
	void HandleDayTimerTick();

};

