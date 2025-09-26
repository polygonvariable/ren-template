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
	FFilterNameRule FilterType = FFilterNameRule();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilterNameRule FilterRarity = FFilterNameRule();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilterIntegerRule FilterRank = FFilterIntegerRule();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilterIntegerRule FilterLevel = FFilterIntegerRule();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilterIntegerRule FilterExperience = FFilterIntegerRule();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFilterIntegerRule FilterQuantity = FFilterIntegerRule();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFilterCombination FilterCombination = EFilterCombination::And;


	RCOREINVENTORY_API bool Match(const FInventoryRecord* Record) const;
	RCOREINVENTORY_API bool Match(const FName& ItemId, const FName& ItemType, const FName& ItemRarity) const;
	RCOREINVENTORY_API bool Match(const FInventoryRecord* Record, const FName& ItemId, const FName& ItemType, const FName& ItemRarity) const;

protected:

	bool MatchInternal(const TArray<bool>& Results) const;

};

