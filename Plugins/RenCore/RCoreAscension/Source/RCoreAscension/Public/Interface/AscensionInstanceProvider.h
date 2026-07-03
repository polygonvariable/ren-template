// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "AscensionInstanceProvider.generated.h"

// Module Macros
#define REN_API RCOREASCENSION_API

// Forward Declarations
struct FAscensionData;


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UAscensionInstanceProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class REN_API IAscensionInstanceProvider
{

	GENERATED_BODY()

public:

	virtual const FAscensionData* GetAscensionInstance(const FPrimaryAssetId& AssetId, const FGuid& InstanceId) const = 0;

};


// Module Macros
#undef REN_API

