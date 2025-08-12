// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenCore/Public/Filter/FilterRule.h"
#include "RenCore/Public/Record/EnhanceRecord.h"

// Generated Headers
#include "InventoryFilterRule.generated.h"

// Forward Declarations
struct FInventoryRecord;



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


	RENCORE_API bool Match(const FInventoryRecord* Record, const FName& ItemId, const uint8& Type, const uint8& Rarity) const;

};

