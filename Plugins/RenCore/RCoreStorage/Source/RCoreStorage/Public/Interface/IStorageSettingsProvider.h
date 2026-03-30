// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "IStorageSettingsProvider.generated.h"

// Module Macros
#define REN_API RCORESTORAGE_API

// Forward Declarations
class UStorage;



UINTERFACE(MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
class UStorageSettingsProvider : public UInterface
{

	GENERATED_BODY()

};

/**
 * 
 */
class REN_API IStorageSettingsProvider
{

	GENERATED_BODY()

public:

	virtual const FName& GetStorageId() const = 0;
	virtual TSubclassOf<UStorage> GetStorageClass() const = 0; 

};



// Module Macros
#undef REN_API

