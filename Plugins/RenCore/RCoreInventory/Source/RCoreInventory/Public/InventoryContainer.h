// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreInventory/Public/InventoryRecord.h"

// Generated Headers
#include "InventoryContainer.generated.h"



/**
 *
 */
USTRUCT(BlueprintType)
struct FInventoryContainer
{

	GENERATED_BODY()

public:

	FInventoryContainer()
	{
		Guid = FGuid::NewGuid();
	}

	UPROPERTY()
	TMap<FName, FInventoryRecord> Items;
	
private:

	UPROPERTY()
	FGuid Guid = FGuid::NewGuid();

	friend inline bool operator == (const FInventoryContainer& A, const FInventoryContainer& B)
	{
		return A.Guid == B.Guid;
	}

	friend inline uint32 GetTypeHash(const FInventoryContainer& Container)
	{
		return GetTypeHash(Container.Guid.ToString());
	}

};

