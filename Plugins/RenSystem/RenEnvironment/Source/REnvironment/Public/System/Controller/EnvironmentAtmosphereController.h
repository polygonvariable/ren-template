
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "System/EnvironmentController.h"

// Generated Headers
#include "EnvironmentAtmosphereController.generated.h"

// Forward Declarations
class USkyAtmosphereComponent;


/**
 *
 */
UCLASS()
class UEnvironmentAtmosphereController : public UEnvironmentStackedController
{

	GENERATED_BODY()

public:

	UEnvironmentAtmosphereController();

	// ~ UEnvironmentStackedController
	virtual void Initialize(AActor* Actor) override;
	virtual void Deinitialize() override;
	// ~ End of UEnvironmentStackedController

protected:

	TWeakObjectPtr<USkyAtmosphereComponent> SkyAtmosphereComponent;
	float CurrentMieScattering = 0.0f;
	float TargetMieScattering = 0.0f;


	// ~ UEnvironmentStackedController
	virtual void OnPriorityItemChanged(UObject* Item) override;
	virtual void OnTransitionChanged(float Alpha) override;
	// ~ End of UEnvironmentStackedController

};

