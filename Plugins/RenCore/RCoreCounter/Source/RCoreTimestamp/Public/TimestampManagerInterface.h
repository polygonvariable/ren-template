// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "TimestampManagerInterface.generated.h"

// Forward Declarations



UINTERFACE(MinimalAPI)
class UTimestampManagerInterface : public UInterface
{
	
	GENERATED_BODY()

};

/**
 *
 */
class RCORETIMESTAMP_API ITimestampManagerInterface
{
	GENERATED_BODY()

public:

	virtual const TMap<FName, FDateTime>& GetTimestamp() const = 0;
	virtual TMap<FName, FDateTime>& GetMutableTimestamp() = 0;

};

