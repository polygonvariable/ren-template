// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "AssetInstanceContextProvider.generated.h"

// Module Macros
#define REN_API RCOREASSETINSTANCE_API


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UAssetInstanceContextProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REN_API IAssetInstanceContextProvider
{

	GENERATED_BODY()

public:

	virtual FGuid GetAssetInstanceId() const = 0;
	virtual FPrimaryAssetId GetAssetId() const = 0;
	virtual FPrimaryAssetType GetAssetType() const = 0;

};


// Module Macros
#undef REN_API

