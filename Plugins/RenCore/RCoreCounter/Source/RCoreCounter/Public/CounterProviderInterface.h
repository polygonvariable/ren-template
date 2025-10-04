// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "CounterProviderInterface.generated.h"

// Forward Declarations
struct FCounterContainer;



UINTERFACE(MinimalAPI)
class UCounterProviderInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCORECOUNTER_API ICounterProviderInterface
{

	GENERATED_BODY()

public:

	virtual TMap<FName, FCounterContainer>& GetCounterContainer() = 0;
	virtual const TMap<FName, FCounterContainer>& GetCounterContainer() const = 0;

};

