
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEnvironment/Public/Controller/EnvironmentController.h"

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

protected:

	TWeakObjectPtr<USkyAtmosphereComponent> SkyAtmosphereComponent;

public:

	// ~ UEnvironmentStackedController
	virtual void Initialize(AActor* Actor) override;
	virtual void Deinitialize() override;
	// ~ End of UEnvironmentStackedController

protected:

	float CurentMieScattering = 0.0f;
	float TargetMieScattering = 0.0f;

	// ~ UEnvironmentStackedController
	virtual void HandleItemChanged(UObject* Item) override;
	virtual void HandleTimerTick(float ElapsedTime) override;
	// ~ End of UEnvironmentStackedController

};

