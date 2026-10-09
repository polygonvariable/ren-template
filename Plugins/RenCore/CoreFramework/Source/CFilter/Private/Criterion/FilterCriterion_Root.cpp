// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Criterion/FilterCriterion_Root.h"

// Project Headers
#include "FilterContext.h"


bool FFilterCriterion_Group::Evaluate(const FFilterContext& Context) const
{
	if (Criteria.Num() == 0)
	{
		return true;
	}

	if (Operator == EFilterOperator::And)
	{
		for (const TInstancedStruct<FFilterCriterion>& RawCriterion : Criteria)
		{
			const FFilterCriterion* Criterion = RawCriterion.GetPtr();
			if (Criterion && !Criterion->Evaluate(Context))
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		for (const TInstancedStruct<FFilterCriterion>& RawCriterion : Criteria)
		{
			const FFilterCriterion* Criterion = RawCriterion.GetPtr();
			if (Criterion && Criterion->Evaluate(Context))
			{
				return true;
			}
		}
		return false;
	}
}

bool FFilterCriterion_Not::Evaluate(const FFilterContext& Context) const
{
	const FFilterCriterion* Criterion = Negate.GetPtr();
	if (Criterion)
	{
		return !Criterion->Evaluate(Context);
	}
	return true;
}

