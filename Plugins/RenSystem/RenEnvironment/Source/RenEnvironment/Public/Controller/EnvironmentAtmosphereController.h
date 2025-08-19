
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

	FName ActorTag = TEXT("Actor.Environment");

protected:

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<USkyAtmosphereComponent> AtmosphereComponent;

public:

	virtual void InitializeController() override;
	virtual void CleanupController() override;

protected:

	virtual void HandleItemChanged(UObject* Item) override;

};

