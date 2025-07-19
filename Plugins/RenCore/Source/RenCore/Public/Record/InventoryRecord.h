// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "InstancedStruct.h"

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


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Record")
	FName ItemId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Record")
	TEnumAsByte<EInventoryItemType> ItemType = EInventoryItemType::Food;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Record")
	int ItemQuantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Record")
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



//USTRUCT(BlueprintType)
//struct FFilterRuleCombination
//{
//
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (BaseStruct = "/Script/RenInventory.FilterRuleCombination"), Category = "Filter Rule Combination")
//	FInstancedStruct A;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter Rule Combination")
//	EFilterCombination Combination = EFilterCombination::And;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (BaseStruct = "/Script/RenInventory.FilterRuleCombination"), Category = "Filter Rule Combination")
//	FInstancedStruct B;
//
//};
//
//USTRUCT(BlueprintType)
//struct FInventoryFilterRules
//{
//
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Filter Rules")
//	TArray<FFilterRuleCombination> Rules;
//
//	bool Matches(const FInventoryRecord& Record, const uint8& Rarity) const
//	{
//		return true;
//	}
//
//};



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


	bool Matches(const FInventoryRecord& Record, const FName& ItemId, const uint8& Type, const uint8& Rarity) const
	{
		bool bPasses = (FilterCombination == EFilterCombination::And);

		if (FilterCombination == EFilterCombination::And)
		{
			if (!FilterId.Matches(ItemId) ||
				!FilterType.Matches(Type) ||
				!FilterRarity.Matches(Rarity) ||
				!FilterRank.Matches(Record.EnhanceRecord.Rank) ||
				!FilterLevel.Matches(Record.EnhanceRecord.Level) ||
				!FilterXp.Matches(Record.EnhanceRecord.Experience) ||
				!FilterQuantity.Matches(Record.ItemQuantity))
			{
				bPasses = false;
			}
		}
		else if (FilterCombination == EFilterCombination::Or)
		{
			if (FilterId.Matches(ItemId) ||
				FilterType.Matches(Type) ||
				FilterRarity.Matches(Rarity) ||
				FilterRank.Matches(Record.EnhanceRecord.Rank) ||
				FilterLevel.Matches(Record.EnhanceRecord.Level) ||
				FilterXp.Matches(Record.EnhanceRecord.Experience) ||
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

