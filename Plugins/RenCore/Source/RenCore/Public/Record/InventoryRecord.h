// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenCore/Public/Filter/FilterRule.h"
#include "RenCore/Public/Inventory/InventoryItemRarity.h"
#include "RenCore/Public/Inventory/InventoryItemType.h"
#include "RenCore/Public/Record/EnhanceRecord.h"

// Generated Headers
#include "InventoryRecord.generated.h"



/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryRecord
{

	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Record")
	FName ItemId;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Record")
	TEnumAsByte<EInventoryItemType> ItemType = EInventoryItemType::Food;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Record")
	int ItemQuantity = 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Record")
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

	UFUNCTION(BlueprintPure, Category = "Inventory Record Library")
	static bool IsValid(const FInventoryRecord& Record) { return Record.IsValid(); }

};



/**
 *
 */
USTRUCT(BlueprintType)
struct FInventoryTable : public FTableRowBase
{

	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Table")
	UPrimaryDataAsset* InventoryAsset = nullptr;

};


/**
 *
 */
USTRUCT(BlueprintType)
struct FInventoryFilterRule
{

	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Filter Rule")
	FFilterNameRule FilterId = FFilterNameRule();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Filter Rule")
	FFilterUInt8Rule FilterType = FFilterUInt8Rule();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Filter Rule")
	FFilterUInt8Rule FilterRarity = FFilterUInt8Rule();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Filter Rule")
	FFilterIntegerRule FilterRank = FFilterIntegerRule();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Filter Rule")
	FFilterIntegerRule FilterLevel = FFilterIntegerRule();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Filter Rule")
	FFilterIntegerRule FilterXp = FFilterIntegerRule();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Filter Rule")
	FFilterIntegerRule FilterQuantity = FFilterIntegerRule();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Filter Rule")
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

