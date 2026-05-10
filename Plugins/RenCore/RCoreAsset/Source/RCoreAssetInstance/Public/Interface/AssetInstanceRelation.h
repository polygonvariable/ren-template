// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "AssetInstanceRelation.generated.h"

// Module Macros
#define REN_API RCOREASSETINSTANCE_API


UINTERFACE(MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
class UAssetInstanceRelation : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REN_API IAssetInstanceRelation
{

	GENERATED_BODY()

public:

	virtual bool HasLinkedInstance(const FPrimaryAssetId& AssetId, const FGuid& AssetInstanceId) const = 0;
	
};


// Module Macros
#undef REN_API

