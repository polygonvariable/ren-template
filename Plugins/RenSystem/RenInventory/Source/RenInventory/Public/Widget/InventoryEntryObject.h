// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "InstancedStruct.h"

// Project Headers
#include "RCorePool/Public/ObjectPoolInterface.h"

// Generated Headers
#include "InventoryEntryObject.generated.h"

// Forward Declarations
class UInventoryAsset;

struct FInventoryRecord;



/**
 *
 */
UCLASS()
class UInventoryEntryObject : public UObject
{

	GENERATED_BODY()

public:

	FPrimaryAssetId AssetId = FPrimaryAssetId();

	int Quantity = 0;

	const FInventoryRecord* Record = nullptr;

	bool bHasPayload = false;

	FInstancedStruct Payload;

	void ResetData()
	{
		AssetId = FPrimaryAssetId();
		Quantity = 0;
		Record = nullptr;

		bHasPayload = false;
		Payload.Reset();
	}
	
	virtual void BeginDestroy() override
	{
		ResetData();
		Super::BeginDestroy();
	}

};

