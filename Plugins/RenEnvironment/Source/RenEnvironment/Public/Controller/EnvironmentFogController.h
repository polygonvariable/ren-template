
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "InstancedStruct.h"

// Project Headers
#include "RenEnvironment/Public/Controller/EnvironmentController.h"
#include "RenEnvironment/Public/Profile/EnvironmentProfile.h"

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

	UPROPERTY()
	FName ActorTag = TEXT("Actor.Environment");

protected:

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UExponentialHeightFogComponent> FogComponent;

public:

	virtual void InitializeController() override;

protected:

	virtual void HandleItemChanged(UObject* Item) override;

};

