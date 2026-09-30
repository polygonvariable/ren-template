// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Generated Headers
#include "AssetEntry.generated.h"


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


	CASSETUI_API virtual FGuid GetAssetInstanceId() const;
	CASSETUI_API virtual void ResetData();

	// ~ UObject
	CASSETUI_API virtual void BeginDestroy() override;
	// ~ End of UObject

};

