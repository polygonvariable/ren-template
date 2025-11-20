
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEnvironment/Public/Controller/EnvironmentController.h"

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

public:

	// ~ UEnvironmentDiscreteController
	virtual void Initialize(AActor* Actor) override;
	virtual void Deinitialize() override;
	// ~ UEnvironmentDiscreteController

};

