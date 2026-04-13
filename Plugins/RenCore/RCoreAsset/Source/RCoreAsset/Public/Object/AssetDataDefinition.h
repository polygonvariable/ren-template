// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "AssetDataDefinition.generated.h"

// Module Macros
#define REN_API RCOREASSET_API


/**
 * 
 */
UCLASS(Const, Abstract, MinimalAPI)
class UAssetDataDefinition : public UObject
{
	
	GENERATED_BODY()

public:

#if WITH_EDITORONLY_DATA
	REN_API virtual void AppendAssetBundleData(FAssetBundleData& InAssetBundleData) {};
#endif

	REN_API static const UAssetDataDefinition* Get()
	{
		return Cast<UAssetDataDefinition>(StaticClass()->GetDefaultObject());
	}

};


// Module Macros
#undef REN_API

