// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "FilterGroup.h"

// Engine Headers

// Project Headers
#include "RCoreFilter/Public/FilterParentCriterion.h"



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

	UFilterGroupCriterion* GroupCriterion = Cast<UFilterGroupCriterion>(Criterion);
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

	UFilterNotCriterion* NotCriterion = Cast<UFilterNotCriterion>(Criterion);
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

