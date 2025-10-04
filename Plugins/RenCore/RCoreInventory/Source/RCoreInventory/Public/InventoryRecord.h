// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "InstancedStruct.h"

// Project Headers
#include "RCoreEnhance/Public/EnhanceRecord.h"

// Generated Headers
#include "InventoryRecord.generated.h"



/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryRecord
{

	GENERATED_BODY()

public:

	/*
	 * 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemId = NAME_None;

	/*
	 * 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FEnhanceRecord Enhancement = FEnhanceRecord();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInstancedStruct Metadata;

	bool IsValid() const
	{
		return ItemId.IsValid() && Quantity > 0;
	}

	friend inline bool operator == (const FInventoryRecord& A, const FInventoryRecord& B)
	{
		return A.ItemId == B.ItemId && A.Quantity == B.Quantity;
	}

	friend inline uint32 GetTypeHash(const FInventoryRecord& Record)
	{
		return HashCombine(GetTypeHash(Record.ItemId.ToString()), GetTypeHash(Record.Quantity));
	}

};

