
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
class IGameClockSubsystemInterface;



/**
 *
 */
UCLASS()
class UEnvironmentDayNightController : public UEnvironmentDiscreteController
{

	GENERATED_BODY()

public:

	FName ActorTag = TEXT("Actor.Environment");
	FName SunComponentTag = TEXT("Environment.Sun");
	FName MoonComponentTag = TEXT("Environment.Moon");

protected:

	UPROPERTY()
	TWeakObjectPtr<UOrbitalLightComponent> SunComponent;

	UPROPERTY()
	TWeakObjectPtr<UOrbitalLightComponent> MoonComponent;

	TWeakInterfacePtr<IGameClockSubsystemInterface> ClockSubsystemInterface;

	FTimerHandle DayTimerHandle;


	bool FindComponents();

	UFUNCTION()
	void StartDayTimer();

	UFUNCTION()
	void StopDayTimer();

	UFUNCTION()
	void HandleDayTimerTick();

public:

	virtual void InitializeController() override;
	virtual void CleanupController() override;

};

