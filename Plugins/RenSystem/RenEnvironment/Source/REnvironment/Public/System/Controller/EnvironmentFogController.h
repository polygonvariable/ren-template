// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "System/EnvironmentController.h"

// Generated Headers
#include "EnvironmentFogController.generated.h"

// Forward Declarations
class UExponentialHeightFogComponent;


/**
 *
 */
UCLASS()
class UEnvironmentFogController : public UEnvironmentStackedController
{

	GENERATED_BODY()

public:

	UEnvironmentFogController();

	// ~ UEnvironmentStackedController
	virtual void Initialize(AActor* Actor) override;
	virtual void Deinitialize() override;
	// ~ End of UEnvironmentStackedController

protected:

	TWeakObjectPtr<UExponentialHeightFogComponent> ExponentialHeightFogComponent;
	float CurrentDensity = 0.0f;
	float TargetDensity = 0.0f;


	// ~ UEnvironmentStackedController
	virtual void OnPriorityItemChanged(UObject* Item) override;
	virtual void OnTransitionChanged(float Alpha) override;
	// ~ End of UEnvironmentStackedController

};

