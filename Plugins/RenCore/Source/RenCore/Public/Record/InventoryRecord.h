// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "Filter/FilterRule.h"
#include "Record/RenRecord.h"
#include "Record/EnhanceRecord.h"
#include "RenCore/Public/Inventory/InventoryItemType.h"
#include "RenCore/Public/Inventory/InventoryItemRarity.h"

// Generated Headers
#include "InventoryRecord.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryRecord
{

	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemId;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EInventoryItemType> ItemType = EInventoryItemType::Food;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ItemQuantity = 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEnhanceRecord EnhanceRecord;


	bool IsValid() const
	{
		return ItemId.IsValid() && ItemQuantity > 0;
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
 * Helper function library for inventory records in Blueprint
 */
UCLASS(MinimalAPI)
class UInventoryRecordLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static bool IsValid(const FInventoryRecord& Record) { return Record.IsValid(); }

};



/**
 *
 */
USTRUCT(BlueprintType)
struct FInventoryTable : public FTableRowBase
{

	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPrimaryDataAsset* InventoryAsset = nullptr;

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FInventoryFilterRule
{

	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilterNameRule FilterId = FFilterNameRule();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilterUInt8Rule FilterType = FFilterUInt8Rule();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilterUInt8Rule FilterRarity = FFilterUInt8Rule();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilterIntegerRule FilterRank = FFilterIntegerRule();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilterIntegerRule FilterLevel = FFilterIntegerRule();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilterIntegerRule FilterXp = FFilterIntegerRule();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilterIntegerRule FilterQuantity = FFilterIntegerRule();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFilterCombination FilterCombination = EFilterCombination::And;


	bool Matches(const FInventoryRecord& Record, const uint8& Rarity) const
	{
		bool bPasses = (FilterCombination == EFilterCombination::And);

		if (FilterCombination == EFilterCombination::And)
		{
			if (!FilterId.Matches(Record.ItemId) || !FilterType.Matches(Record.ItemType) ||
				!FilterRarity.Matches(Rarity) || !FilterRank.Matches(Record.EnhanceRecord.Rank) ||
				!FilterLevel.Matches(Record.EnhanceRecord.Level) || !FilterXp.Matches(Record.EnhanceRecord.Experience) ||
				!FilterQuantity.Matches(Record.ItemQuantity))
			{
				bPasses = false;
			}
		}
		else if (FilterCombination == EFilterCombination::Or)
		{
			if (FilterId.Matches(Record.ItemId) || FilterType.Matches(Record.ItemType) ||
				FilterRarity.Matches(Rarity) || FilterRank.Matches(Record.EnhanceRecord.Rank) ||
				FilterLevel.Matches(Record.EnhanceRecord.Level) || FilterXp.Matches(Record.EnhanceRecord.Experience) ||
				FilterQuantity.Matches(Record.ItemQuantity))
			{
				bPasses = true;
			}
			else
			{
				bPasses = false;
			}
		}

		return bPasses;
	}

};

