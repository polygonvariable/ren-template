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
struct FInventoryStack
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FInventoryRecord> ItemRecords;

	UPROPERTY()
	FGuid StackId;

	UPROPERTY()
	bool bStackable = false;

	UPROPERTY()
	bool bPersistWhenEmpty = false;

	FInventoryStack() {}
	FInventoryStack(FGuid InStackId, bool bInStackable, bool bInPersistWhenEmpty) : StackId(InStackId), bStackable(bInStackable), bPersistWhenEmpty(bInPersistWhenEmpty) {}

	friend inline bool operator == (const FInventoryStack& A, const FInventoryStack& B)
	{
		return A.StackId == B.StackId;
	}

	friend inline uint32 GetTypeHash(const FInventoryStack& A)
	{
		return GetTypeHash(A.StackId.ToString());
	}

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FInventoryContainer
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<FName, FInventoryStack> Stacks;

	UPROPERTY()
	FGuid ContainerId;

	FInventoryContainer() {}
	FInventoryContainer(const FGuid& InContainerId) : ContainerId(InContainerId) {}

	friend inline bool operator == (const FInventoryContainer& A, const FInventoryContainer& B)
	{
		return A.ContainerId == B.ContainerId;
	}

	friend inline uint32 GetTypeHash(const FInventoryContainer& A)
	{
		return GetTypeHash(A.ContainerId.ToString());
	}

};

