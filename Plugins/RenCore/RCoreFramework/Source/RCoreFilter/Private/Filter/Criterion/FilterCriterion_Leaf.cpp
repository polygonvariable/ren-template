// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Filter/Criterion/FilterCriterion_Leaf.h"

// Engine Headers

// Project Headers
#include "Definition/FilterContext.h"



FName UFilterCriterion_Leaf::GetPropertyName() const
{
	return PropertyName;
}

bool UFilterCriterion_Leaf::GetIsLeaf() const
{
	return true;
}


bool UFilterCriterion_Text::Evaluate(const FFilterContext& Context) const
{
	FName Value;
	if (!Context.GetValue(PropertyName, Value))
	{
		return false;
	}
	return Included.Contains(Value);
}

void UFilterCriterion_Text::ClearEvaluationData()
{
	Included.Empty();
}

void UFilterCriterion_Text::CopyEvaluationData(const UFilterCriterion* Other)
{
	const UFilterCriterion_Text* OtherCriterion = Cast<UFilterCriterion_Text>(Other);
	if (OtherCriterion)
	{
		Included.Append(OtherCriterion->Included);
	}
}


bool UFilterCriterion_Guid::Evaluate(const FFilterContext& Context) const
{
	FName Value;
	if (!Context.GetValue(PropertyName, Value))
	{
		return false;
	}
	return Included.Contains(FGuid(Value.ToString()));
}

void UFilterCriterion_Guid::ClearEvaluationData()
{
	Included.Empty();
}

void UFilterCriterion_Guid::CopyEvaluationData(const UFilterCriterion* Other)
{
	const UFilterCriterion_Guid* OtherCriterion = Cast<UFilterCriterion_Guid>(Other);
	if (OtherCriterion)
	{
		Included.Append(OtherCriterion->Included);
	}
}


bool UFilterCriterion_Asset::Evaluate(const FFilterContext& Context) const
{
	FPrimaryAssetId Value;
	if (!Context.GetValue(PropertyName, Value))
	{
		return false;
	}
	return Included.Contains(Value);
}

void UFilterCriterion_Asset::ClearEvaluationData()
{
	Included.Empty();
}

void UFilterCriterion_Asset::CopyEvaluationData(const UFilterCriterion* Other)
{
	const UFilterCriterion_Asset* OtherCriterion = Cast<UFilterCriterion_Asset>(Other);
	if (OtherCriterion)
	{
		Included.Append(OtherCriterion->Included);
	}
}


bool UFilterCriterion_Integer::Evaluate(const FFilterContext& Context) const
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

void UFilterCriterion_Integer::ClearEvaluationData()
{
	Min = 0;
	Max = 0;
	bEnableStrictMode = false;
}

void UFilterCriterion_Integer::CopyEvaluationData(const UFilterCriterion* Other)
{
	const UFilterCriterion_Integer* OtherCriterion = Cast<UFilterCriterion_Integer>(Other);
	if (OtherCriterion)
	{
		Min = OtherCriterion->Min;
		Max = OtherCriterion->Max;
		bEnableStrictMode = OtherCriterion->bEnableStrictMode;
	}
}

