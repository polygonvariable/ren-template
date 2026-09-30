// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers
#include "SpawnDataSource.h"

// Generated Headers
#include "SpawnContextProvider.generated.h"

// Forward Declarations
struct FGameplayTag;


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class USpawnContextProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CGAMEDATASPAWN_API ISpawnContextProvider
{

	GENERATED_BODY()

public:

	virtual ESpawnDataSource GetSpawnSource() const { return ESpawnDataSource::Static; };

};

