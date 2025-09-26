// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InventoryFilterRule.h"

// Engine Headers

// Project Headers
#include "RCoreEnhance/Public/EnhanceRecord.h"

#include "RCoreInventory/Public/InventoryRecord.h"



bool FInventoryFilterRule::Match(const FInventoryRecord* Record) const
{
	TArray<bool> Results;

	if (Record)
	{
		Results.Add(FilterRank.Matches(Record->EnhanceRecord.Rank));
		Results.Add(FilterLevel.Matches(Record->EnhanceRecord.Level));
		Results.Add(FilterExperience.Matches(Record->EnhanceRecord.Experience));
		Results.Add(FilterQuantity.Matches(Record->ItemQuantity));
	}

	return MatchInternal(Results);
}

bool FInventoryFilterRule::Match(const FName& ItemId, const FName& ItemType, const FName& ItemRarity) const
{
	TArray<bool> Results;
	Results.Add(FilterId.Matches(ItemId));
	Results.Add(FilterType.Matches(ItemType));
	Results.Add(FilterRarity.Matches(ItemRarity));

	return MatchInternal(Results);
}

bool FInventoryFilterRule::Match(const FInventoryRecord* Record, const FName& ItemId, const FName& ItemType, const FName& ItemRarity) const
{
	return Match(Record) && Match(ItemId, ItemType, ItemRarity);
}

bool FInventoryFilterRule::MatchInternal(const TArray<bool>& Results) const
{
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
