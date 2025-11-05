// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreCommon/Public/Priority/PrioritySystem.h"
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"

// Generated Headers
#include "EnvironmentController.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(Abstract)
class UEnvironmentDiscreteController : public UObject
{

	GENERATED_BODY()

public:

	virtual void InitializeController(AActor* Actor);
	virtual void CleanupController();

};


/**
 *
 */
UCLASS(Abstract)
class UEnvironmentStackedController : public UPrioritySystem
{

	GENERATED_BODY()

public:

	EEnvironmentProfileType ProfileType;

	virtual void InitializeController(AActor* Actor);
	virtual void CleanupController();

};


