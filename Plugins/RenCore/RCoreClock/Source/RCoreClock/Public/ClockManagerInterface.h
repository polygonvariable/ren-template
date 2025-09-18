// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers
#include "RCoreClock/Public/ClockDelegates.h"

// Generated Headers
#include "ClockManagerInterface.generated.h"

// Forward Declarations



UINTERFACE(MinimalAPI)
class UClockManagerInterface : public UInterface
{
	
	GENERATED_BODY()

};

/**
 * 
 */
class RCORECLOCK_API IClockManagerInterface
{

	GENERATED_BODY()

public:

	virtual FClockDelegates& GetClockDelegates() = 0;

	virtual float GetSmoothNormalizedTime() const = 0;
	virtual bool IsClockActive() const = 0;

};

