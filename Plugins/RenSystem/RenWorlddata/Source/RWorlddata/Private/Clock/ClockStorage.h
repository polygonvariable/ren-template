// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Clock/ClockInstance.h"

// Generated Headers
#include "ClockStorage.generated.h"


/**
 *
 */
UCLASS()
class UClockStorage : public UObject
{

	GENERATED_BODY()

public:

	/** TMap<WorldId, ClockInstance> */
	UPROPERTY(SaveGame)
	TMap<FName, FClockInstance> Clocks;

};

