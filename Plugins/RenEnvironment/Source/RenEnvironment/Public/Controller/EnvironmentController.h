// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenCore/Public/Priority/PrioritySystem.h"
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"

// Generated Headers
#include "EnvironmentController.generated.h"

// Forward Declarations
class UTimer;



/**
 *
 */
UCLASS(Abstract)
class UEnvironmentDiscreteController : public UObject
{

	GENERATED_BODY()

public:

	virtual void InitializeController();
	virtual void CleanupController();

};


/**
 *
 */
UCLASS(Abstract)
class UEnvironmentStackedController : public UObjectPrioritySystem
{

	GENERATED_BODY()

public:

	virtual void InitializeController();
	virtual void CleanupController();

};


