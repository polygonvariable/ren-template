// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "AssetDataDefinition.generated.h"


/**
 * 
 */
UCLASS(Const, Abstract, MinimalAPI)
class UAssetDataDefinition : public UObject
{
	
	GENERATED_BODY()

public:

#if WITH_EDITOR
	// ~ UObject
	CASSET_API virtual void AppendAssetBundleData(FAssetBundleData& InAssetBundleData) {};
	// ~ End of UObject
#endif

public:

	static const UAssetDataDefinition* Get()
	{
		return Cast<UAssetDataDefinition>(StaticClass()->GetDefaultObject());
	}

};

