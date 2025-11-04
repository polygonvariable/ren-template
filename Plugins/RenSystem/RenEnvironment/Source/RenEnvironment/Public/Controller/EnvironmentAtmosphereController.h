
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
	virtual void InitializeController(AActor* Actor) override;
	virtual void CleanupController() override;
	// ~ End of UEnvironmentStackedController

protected:

	// ~ UObjectPrioritySystem
	virtual void HandleItemChanged(UObject* Item) override;
	// ~ End of UObjectPrioritySystem

};

