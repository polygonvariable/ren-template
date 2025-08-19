// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "InstancedStruct.h"

// Project Headers
#include "RenCore/Public/Interface/ObjectPoolInterface.h"

// Generated Headers
#include "InventoryEntryObject.generated.h"

// Forward Declarations
class UInventoryAsset;

struct FInventoryRecord;



/**
 *
 */
UCLASS()
class UInventoryEntryObject : public UObject, public IObjectPoolInterface
{

	GENERATED_BODY()

public:

	FName ItemGuid = NAME_None;
	TObjectPtr<UInventoryAsset> InventoryAsset = nullptr;
	const FInventoryRecord* InventoryRecord = nullptr;

	bool bEnablePayload = false;
	FInstancedStruct InventoryPayload;

	void ResetData()
	{
		ItemGuid = NAME_None;
		InventoryAsset = nullptr;
		InventoryRecord = nullptr;
		bEnablePayload = false;
		InventoryPayload.Reset();
	}
	
	virtual void ReturnToPool() override
	{
		ResetData();
	}

	virtual void BeginDestroy() override
	{
		ResetData();
		UE_LOG(LogTemp, Warning, TEXT("Object Destroyed"));
		Super::BeginDestroy();
	}

};

