// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "InstancedStruct.h"

// Project Headers
#include "RenCore/Public/Filter/FilterRule.h"
#include "RenCore/Public/Inventory/InventoryItemType.h"
#include "RenCore/Public/Record/EnhanceRecord.h"

// Generated Headers
#include "InventoryRecord.generated.h"



// DECLARE_MULTICAST_DELEGATE(FOnRecordUpdated);


/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryRecord
{

	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Record")
	FName ItemId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Record")
	TEnumAsByte<EInventoryItemType> ItemType = EInventoryItemType::Food;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Record")
	int ItemQuantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Record")
	FEnhanceRecord EnhanceRecord;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Record")
	FTimespan ExpireTime = FTimespan::Zero();


	bool IsValid() const
	{
		return ItemId.IsValid() && ItemQuantity > 0;
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

	const FDateTime GetExpireDate() const
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



/**
 *
 */
USTRUCT(BlueprintType)
struct FInventoryContainer
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<FName, FInventoryRecord> Items;
	// TMultiMap<FName, FInventoryRecord> MultiItems;

private:

	UPROPERTY()
	FGuid Id = FGuid::NewGuid();

	friend inline bool operator == (const FInventoryContainer& A, const FInventoryContainer& B)
	{
		return A.Id == B.Id;
	}

	friend inline uint32 GetTypeHash(const FInventoryContainer& Container)
	{
		return GetTypeHash(Container.Id.ToString());
	}

};

