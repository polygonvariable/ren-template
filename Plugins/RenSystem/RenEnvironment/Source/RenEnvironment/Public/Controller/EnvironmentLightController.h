
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEnvironment/Public/Controller/EnvironmentController.h"

// Generated Headers
#include "EnvironmentLightController.generated.h"

// Forward Declarations
class UDirectionalLightComponent;



/**
 *
 */
UCLASS()
class UEnvironmentLightController : public UEnvironmentStackedController
{

	GENERATED_BODY()

public:

	UEnvironmentLightController();

	FName SunTag = TEXT("Environment.Sun");
	FName MoonTag = TEXT("Environment.Moon");

protected:

	TWeakObjectPtr<UDirectionalLightComponent> SunComponent;
	TWeakObjectPtr<UDirectionalLightComponent> MoonComponent;

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

