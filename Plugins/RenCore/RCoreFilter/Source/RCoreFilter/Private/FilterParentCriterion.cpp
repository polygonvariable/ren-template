// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "FilterParentCriterion.h"

// Engine Headers

// Project Headers



bool UFilterGroupCriterion::Evaluate(const FFilterContext& Context) const
{
	if (Criteria.Num() == 0)
	{
		return true;
	}

	if (Combination == EFilterCombination::And)
	{
		for (const UFilterCriterion* Criterion : Criteria)
		{
			if (Criterion && !Criterion->Evaluate(Context))
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		for (const UFilterCriterion* Criterion : Criteria)
		{
			if (Criterion && Criterion->Evaluate(Context))
			{
				return true;
			}
		}
		return false;
	}
}

bool UFilterNotCriterion::Evaluate(const FFilterContext& Context) const
{
	if (Negate)
	{
		return !Negate->Evaluate(Context);
	}
	return true;
}

