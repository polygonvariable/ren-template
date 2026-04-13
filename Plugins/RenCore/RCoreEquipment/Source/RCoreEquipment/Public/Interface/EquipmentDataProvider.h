// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "EquipmentDataProvider.generated.h"

// Module Macros
#define REN_API RCOREEQUIPMENT_API

// Forward Declarations
class UPrimaryDataAsset;


UINTERFACE(MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
class UEquipmentDataProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REN_API IEquipmentDataProvider
{

	GENERATED_BODY()

public:

	//virtual const FSoftClassPath& GetEquipmentActorClass() const = 0;
	//virtual const TSoftObjectPtr<UPrimaryDataAsset>& GetEquipmentDataAsset() const = 0;

};


// Module Macros
#undef REN_API

