// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenCore/Public/Interface/ObjectPoolInterface.h"
#include "RenCore/Public/Record/InventoryRecord.h"

// Generated Headers
#include "InventoryEntryObject.generated.h"

// Forward Declarations
class UInventoryAsset;



/**
 *
 */
UCLASS()
class UInventoryEntryObject : public UObject, public IObjectPoolInterface
{

	GENERATED_BODY()

public:

	FName ItemGuid = NAME_None;
	UInventoryAsset* InventoryAsset = nullptr;
	const FInventoryRecord* InventoryRecord = nullptr;

	void ResetData()
	{
		ItemGuid = NAME_None;
		InventoryAsset = nullptr;
		InventoryRecord = nullptr;
	}
	
	virtual void ReturnToPool() override
	{
		ResetData();
	}

	virtual void BeginDestroy() override
	{
		ResetData();
		Super::BeginDestroy();
	}

};

