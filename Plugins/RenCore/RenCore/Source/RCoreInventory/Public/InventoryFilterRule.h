// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreCommon/Public/Filter/FilterRule.h"

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


	RCOREINVENTORY_API bool Match(const FInventoryRecord* Record, const FName& ItemId, const uint8& Type, const uint8& Rarity) const;

};

