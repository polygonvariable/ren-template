// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InventoryFilterRule.h"

// Engine Headers

// Project Headers
#include "RCoreEnhance/Public/EnhanceRecord.h"

#include "RCoreInventory/Public/InventoryRecord.h"



bool FInventoryFilterRule::Match(const FInventoryRecord* Record, const FName& ItemId, const uint8& Type, const uint8& Rarity) const
{
	TArray<bool> Results;
	Results.Add(FilterId.Matches(ItemId));
	Results.Add(FilterType.Matches(Type));
	Results.Add(FilterRarity.Matches(Rarity));

	if (Record)
	{
		Results.Add(FilterRank.Matches(Record->EnhanceRecord.Rank));
		Results.Add(FilterLevel.Matches(Record->EnhanceRecord.Level));
		Results.Add(FilterXp.Matches(Record->EnhanceRecord.Experience));
		Results.Add(FilterQuantity.Matches(Record->ItemQuantity));
	}

	if (FilterCombination == EFilterCombination::And)
	{
		for (bool Result : Results)
		{
			if (!Result)
			{
				return false;
			}
		}
		return true;
	}
	else if (FilterCombination == EFilterCombination::Or)
	{
		for (bool Result : Results)
		{
			if (Result)
			{
				return true;
			}
		}
		return false;
	}

	return false;
}
