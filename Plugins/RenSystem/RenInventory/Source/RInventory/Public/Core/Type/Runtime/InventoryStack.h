// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/Runtime/InventoryInstance.h"

// Generated Headers
#include "InventoryStack.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FInventoryStack
{

	GENERATED_BODY()

public:

	FInventoryStack() {}
	FInventoryStack(FGuid InStackId, bool bInStackable, bool bInPersistWhenEmpty) : StackId(InStackId), bStackable(bInStackable), bPersistWhenEmpty(bInPersistWhenEmpty) {}


	UPROPERTY(EditAnywhere, SaveGame)
	TArray<FInventoryInstance> ItemList;

	UPROPERTY(EditAnywhere, SaveGame)
	FGuid StackId;

	UPROPERTY(EditAnywhere, SaveGame)
	bool bStackable = false;

	UPROPERTY(EditAnywhere, SaveGame)
	bool bPersistWhenEmpty = false;


	friend inline bool operator == (const FInventoryStack& A, const FInventoryStack& B)
	{
		return A.StackId == B.StackId;
	}

	friend inline uint32 GetTypeHash(const FInventoryStack& A)
	{
		return GetTypeHash(A.StackId.ToString());
	}

};

