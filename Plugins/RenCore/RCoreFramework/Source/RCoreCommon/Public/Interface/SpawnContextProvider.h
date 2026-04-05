// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "SpawnContextProvider.generated.h"

// Module Macros
#define REN_API RCORECOMMON_API

// Forward Declarations
struct FGameplayTag;
struct FInstancedStruct;


UINTERFACE(MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
class USpawnContextProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REN_API ISpawnContextProvider
{

	GENERATED_BODY()

public:

	virtual void GetSpawnData(const FGameplayTag& InTag, FInstancedStruct& OutValue) const = 0;

};


// Module Macros
#undef REN_API

