// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "FilterGroup.h"

// Project Headers
#include "Criterion/FilterCriterion_Root.h"


TInstancedStruct<FFilterCriterion>* FFilterGroup::GetCriterionByName(FName PropertyName)
{
	if (!CriterionRoot.IsValid())
	{
		return nullptr;
	}
	return FindCriterionByName(CriterionRoot, PropertyName);
}

TInstancedStruct<FFilterCriterion>* FFilterGroup::FindCriterionByName(TInstancedStruct<FFilterCriterion>& Criterion, const FName& PropertyName)
{
	FFilterCriterion* BaseCriterion = Criterion.GetMutablePtr();
	if (!BaseCriterion)
	{
		return nullptr;
	}

	if (BaseCriterion->GetIsLeaf() && BaseCriterion->GetPropertyName().IsEqual(PropertyName))
	{
		return &Criterion;
	}

	FFilterCriterion_Group* GroupCriterion = Criterion.GetMutablePtr<FFilterCriterion_Group>();
	if (GroupCriterion)
	{
		TArray<TInstancedStruct<FFilterCriterion>>& Criteria = GroupCriterion->Criteria;
		for (TInstancedStruct<FFilterCriterion>& Child : Criteria)
		{
			return FindCriterionByName(Child, PropertyName);
		}
	}

	FFilterCriterion_Not* NotCriterion = Criterion.GetMutablePtr<FFilterCriterion_Not>();
	if (NotCriterion)
	{
		return FindCriterionByName(NotCriterion->Negate, PropertyName);
	}

	return nullptr;
}

