// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers
#include "RCoreClock/Public/ClockRecord.h"

// Generated Headers
#include "ClockProviderInterface.generated.h"

// Forward Declarations



UINTERFACE(MinimalAPI)
class UClockProviderInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCORECLOCK_API IClockProviderInterface
{
	
	GENERATED_BODY()

public:

	virtual const TMap<FName, FClockRecord>& GetClockRecords() const = 0;
	virtual TMap<FName, FClockRecord>& GetMutableClockRecords() = 0;
	
};

