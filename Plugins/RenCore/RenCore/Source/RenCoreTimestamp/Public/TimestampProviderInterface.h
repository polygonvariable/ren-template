// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "TimestampProviderInterface.generated.h"

// Forward Declarations



UINTERFACE(MinimalAPI)
class UTimestampProviderInterface : public UInterface
{
	
	GENERATED_BODY()

};

/**
 *
 */
class ITimestampProviderInterface
{
	GENERATED_BODY()

public:

	RENCORETIMESTAMP_API virtual const TMap<FName, FDateTime>& GetTimestamp() const = 0;
	RENCORETIMESTAMP_API virtual TMap<FName, FDateTime>& GetMutableTimestamp() = 0;

};

