// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreInventory/Public/InventoryItemType.h"
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
	 * Static ID of the item, based on the asset.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EInventoryItemType> ItemType = EInventoryItemType::Food;

	// TODO:
	// Rename this from ItemQuantity to Quantity
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ItemQuantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FEnhanceRecord EnhanceRecord = FEnhanceRecord();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTimespan ExpireTime = FTimespan::Zero();


	bool IsValid() const
	{
		return ItemId.IsValid() && ItemQuantity > 0 && !IsExpired();
	}

	bool CanExpire() const
	{
		return ExpireTime.IsZero();
	}

	bool IsExpired() const
	{
		if (!CanExpire()) return false;

		FDateTime CurrentDate = FDateTime::Now();
		FDateTime ExpireDate = CurrentDate + ExpireTime;
		return ExpireDate < CurrentDate;
	}

	const FDateTime GetExpireDateTime() const
	{
		return FDateTime::Now() + ExpireTime;
	}

	friend inline bool operator == (const FInventoryRecord& A, const FInventoryRecord& B)
	{
		return A.ItemId == B.ItemId && A.ItemType == B.ItemType;
	}

	friend inline uint32 GetTypeHash(const FInventoryRecord& Record)
	{
		return HashCombine(GetTypeHash(Record.ItemId.ToString()), GetTypeHash(Record.ItemType));
	}

};

