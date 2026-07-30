// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Generated Headers
#include "AssetEntry.generated.h"

// Module Macros
#define REN_API RCOREASSETUI_API



/**
 *
 */
UCLASS(MinimalAPI)
class UAssetEntry : public UObject
{

	GENERATED_BODY()

public:

	FPrimaryAssetId AssetId = FPrimaryAssetId();
	FInstancedStruct AssetSubDetail;


	REN_API virtual FGuid GetAssetInstanceId() const;
	REN_API virtual void ResetData();

	// ~ UObject
	REN_API virtual void BeginDestroy() override;
	// ~ End of UObject

};


// Module Macros
#undef REN_API

