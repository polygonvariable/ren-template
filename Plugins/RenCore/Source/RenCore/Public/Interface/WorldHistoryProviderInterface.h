// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "WorldHistoryProviderInterface.generated.h"

// Forward Declarations



UINTERFACE(MinimalAPI)
class UWorldHistoryProviderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class RENCORE_API IWorldHistoryProviderInterface
{
	GENERATED_BODY()

public:

	virtual const TMap<FName, FDateTime>& GetWorldHistory() const = 0;
	virtual TMap<FName, FDateTime>& GetMutableWorldHistory() = 0;

};

