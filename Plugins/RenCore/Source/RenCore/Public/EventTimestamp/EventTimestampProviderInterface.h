// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "EventTimestampProviderInterface.generated.h"

// Forward Declarations



UINTERFACE(MinimalAPI)
class UEventTimestampProviderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class RENCORE_API IEventTimestampProviderInterface
{
	GENERATED_BODY()

public:

	virtual const TMap<FName, FDateTime>& GetEventTimestamp() const = 0;
	virtual TMap<FName, FDateTime>& GetMutableEventTimestamp() = 0;

};

