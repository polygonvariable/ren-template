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

	FName ActorTag = TEXT("Actor.Environment");

protected:

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UExponentialHeightFogComponent> FogComponent;

public:

	virtual void InitializeController() override;
	virtual void CleanupController() override;

protected:

	virtual void HandleItemChanged(UObject* Item) override;

};

