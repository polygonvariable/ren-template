// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "IEquipmentProvider.generated.h"

// Module Macros
#define REN_API RCOREEQUIPMENT_API

// Forward Declarations
struct FGameplayTag;



UINTERFACE(MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
class UEquipmentProvider : public UInterface
{

	GENERATED_BODY()

};

/**
 * 
 */
class REN_API IEquipmentProvider
{

	GENERATED_BODY()

public:

	virtual const TSoftClassPtr<AActor>& GetEquipmentClass() const = 0;
	virtual const TArray<FSoftClassPath>& GetEquipmentAbilities() const = 0;
	virtual const FName& GetEquipmentSocket() const = 0;

};



// Module Macros
#undef REN_API

