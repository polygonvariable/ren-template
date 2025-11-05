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
	virtual void InitializeController(AActor* Actor) override;
	virtual void CleanupController() override;
	// ~ End of UEnvironmentStackedController

protected:

	// ~ UPrioritySystem
	virtual void OnItemChanged(UObject* Item) override;
	// ~ End of UPrioritySystem

};

