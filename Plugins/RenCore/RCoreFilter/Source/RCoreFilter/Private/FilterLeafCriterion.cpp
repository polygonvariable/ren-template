// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "FilterLeafCriterion.h"

// Engine Headers

// Project Headers
#include "RCoreFilter/Public/FilterContext.h"



FName UFilterLeafCriterion::GetPropertyName() const
{
	return PropertyName;
}

bool UFilterLeafCriterion::GetIsLeaf() const
{
	return true;
}

bool UFilterTextCriterion::Evaluate(const FFilterContext& Context) const
{
	FName Value;
	if (!Context.GetValue(PropertyName, Value))
	{
		return false;
	}
	return Included.Contains(Value);
}

bool UFilterAssetCriterion::Evaluate(const FFilterContext& Context) const
{
	FPrimaryAssetId Value;
	if (!Context.GetValue(PropertyName, Value))
	{
		return false;
	}
	return Included.Contains(Value);
}

bool UFilterIntegerCriterion::Evaluate(const FFilterContext& Context) const
{
	int Value;
	if (!Context.GetValue(PropertyName, Value))
	{
		return false;
	}

	if (bEnableStrictMode)
	{
		return Value > Min && Value < Max;
	}
	else
	{
		return Value >= Min && Value <= Max;
	}
}

