
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEnvironment/Public/Controller/EnvironmentController.h"
#include "RenEnvironment/Public/Profile/EnvironmentProfile.h"

// Generated Headers
#include "EnvironmentDayNightController.generated.h"

// Forward Declarations
class UOrbitalLightComponent;
class UGameClockSubsystem;
class IGameClockSubsystemInterface;


/**
 *
 */
UCLASS()
class UEnvironmentDayNightController : public UEnvironmentDiscreteController
{

	GENERATED_BODY()

public:

	UPROPERTY()
	FName ActorTag = TEXT("Actor.Environment");

	UPROPERTY()
	FName SunComponentName = TEXT("Environment.Sun");

	UPROPERTY()
	FName MoonComponentName = TEXT("Environment.Moon");

protected:

	UPROPERTY()
	TObjectPtr<UGameClockSubsystem> GameClockSubsystem;

	UPROPERTY()
	TObjectPtr<UTimer> DayTimer;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UOrbitalLightComponent> SunComponent;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UOrbitalLightComponent> MoonComponent;


	TWeakInterfacePtr<IGameClockSubsystemInterface> GameClockSubsystemInterface;


	UFUNCTION()
	void StartDayTimer();

	UFUNCTION()
	void StopDayTimer();

	UFUNCTION()
	void HandleDayTimerTick(float ElapsedTime);

public:

	virtual void InitializeController() override;
	virtual void CleanupController() override;

};

