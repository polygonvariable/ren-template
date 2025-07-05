
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "InstancedStruct.h"

// Project Headers
#include "RenEnvironment/Public/Controller/EnvironmentController.h"
#include "RenEnvironment/Public/Profile/EnvironmentProfile.h"

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

	UPROPERTY()
	FName ActorTag = TEXT("Actor.Environment");

	UPROPERTY()
	FName SunComponentName = TEXT("Environment.Sun");

	UPROPERTY()
	FName MoonComponentName = TEXT("Environment.Moon");

protected:

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UDirectionalLightComponent> SunComponent;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UDirectionalLightComponent> MoonComponent;

public:

	virtual void InitializeController() override;

protected:

	virtual void HandleItemChanged(UObject* Item) override;

};

