// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Control/AssetFilterSlot.h"

// Project Headers
#include "Definition/FilterContext.h"
#include "Filter/FilterCriterion.h"
#include "Filter/FilterGroup.h"



void UAssetFilterSlot::Evaluate(const FFilterContext& Context)
{
	if (!IsValid(FilterGroup))
	{
		return;
	}

	UFilterCriterion* Criterion = FilterGroup->CriterionRoot;
	if (!IsValid(Criterion))
	{
		return;
	}

	if (Criterion->Evaluate(Context))
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

