// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEnvironment/Public/Controller/EnvironmentController.h"

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

protected:

	TWeakObjectPtr<UExponentialHeightFogComponent> ExponentialHeightFogComponent;

public:

	// ~ UEnvironmentStackedController
	virtual void Initialize(AActor* Actor) override;
	virtual void Deinitialize() override;
	// ~ End of UEnvironmentStackedController

protected:

	float CurentDensity = 0.0f;
	float TargetDensity = 0.0f;

	// ~ UEnvironmentStackedController
	virtual void HandleItemChanged(UObject* Item) override;
	virtual void HandleTimerTick(float ElapsedTime) override;
	// ~ End of UEnvironmentStackedController

};

