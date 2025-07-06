
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

	FName ActorTag = TEXT("Actor.Environment");
	FName SunComponentTag = TEXT("Environment.Sun");
	FName MoonComponentTag = TEXT("Environment.Moon");

protected:

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UDirectionalLightComponent> SunComponent;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UDirectionalLightComponent> MoonComponent;

public:

	virtual void InitializeController() override;
	virtual void CleanupController() override;

protected:

	virtual void HandleItemChanged(UObject* Item) override;

};

