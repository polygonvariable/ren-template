// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Filter/FilterGroup.h"

// Engine Headers

// Project Headers
#include "Filter/Criterion/FilterCriterion_Root.h"



UFilterCriterion* UFilterGroup::FindCriterionByName(UFilterCriterion* Criterion, const FName& PropertyName) const
{
	if (!Criterion)
	{
		return nullptr;
	}

	if (Criterion->GetIsLeaf() && Criterion->GetPropertyName() == PropertyName)
	{
		return Criterion;
	}

	UFilterCriterion_Group* GroupCriterion = Cast<UFilterCriterion_Group>(Criterion);
	if (IsValid(GroupCriterion))
	{
		TArray<UFilterCriterion*> Criteria = GroupCriterion->Criteria;
		for (UFilterCriterion* Child : Criteria)
		{
			UFilterCriterion* Found = FindCriterionByName(Child, PropertyName);
			if (IsValid(Found))
			{
				return Found;
			}
		}
	}

	UFilterCriterion_Not* NotCriterion = Cast<UFilterCriterion_Not>(Criterion);
	if (IsValid(NotCriterion))
	{
		return FindCriterionByName(NotCriterion->Negate, PropertyName);
	}

	return nullptr;
}

UFilterCriterion* UFilterGroup::GetCriterionByName(FName PropertyName) const
{
	if (!CriterionRoot)
	{
		return nullptr;
	}
	return FindCriterionByName(CriterionRoot, PropertyName);
}

