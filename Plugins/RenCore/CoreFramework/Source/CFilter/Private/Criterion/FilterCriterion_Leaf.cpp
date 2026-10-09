// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Criterion/FilterCriterion_Leaf.h"

// Project Headers
#include "FilterContext.h"


FName FFilterCriterion_Leaf::GetPropertyName() const
{
	return PropertyName;
}

bool FFilterCriterion_Leaf::GetIsLeaf() const
{
	return true;
}


bool FFilterCriterion_Text::Evaluate(const FFilterContext& Context) const
{
	FName Value;
	if (!Context.GetValue(PropertyName, Value))
	{
		return false;
	}
	return Included.Contains(Value);
}

void FFilterCriterion_Text::ClearEvaluationData()
{
	Included.Empty();
}

void FFilterCriterion_Text::CopyEvaluationData(const TInstancedStruct<FFilterCriterion>& Other)
{
	const FFilterCriterion_Text* OtherCriterion = Other.GetPtr<FFilterCriterion_Text>();
	if (OtherCriterion)
	{
		Included.Append(OtherCriterion->Included);
	}
}


bool FFilterCriterion_Guid::Evaluate(const FFilterContext& Context) const
{
	FName Value;
	if (!Context.GetValue(PropertyName, Value))
	{
		return false;
	}
	return Included.Contains(FGuid(Value.ToString()));
}

void FFilterCriterion_Guid::ClearEvaluationData()
{
	Included.Empty();
}

void FFilterCriterion_Guid::CopyEvaluationData(const TInstancedStruct<FFilterCriterion>& Other)
{
	const FFilterCriterion_Guid* OtherCriterion = Other.GetPtr<FFilterCriterion_Guid>();
	if (OtherCriterion)
	{
		Included.Append(OtherCriterion->Included);
	}
}


bool FFilterCriterion_Asset::Evaluate(const FFilterContext& Context) const
{
	FPrimaryAssetId Value;
	if (!Context.GetValue(PropertyName, Value))
	{
		return false;
	}
	return Included.Contains(Value);
}

void FFilterCriterion_Asset::ClearEvaluationData()
{
	Included.Empty();
}

void FFilterCriterion_Asset::CopyEvaluationData(const TInstancedStruct<FFilterCriterion>& Other)
{
	const FFilterCriterion_Asset* OtherCriterion = Other.GetPtr<FFilterCriterion_Asset>();
	if (OtherCriterion)
	{
		Included.Append(OtherCriterion->Included);
	}
}


bool FFilterCriterion_Integer::Evaluate(const FFilterContext& Context) const
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

void FFilterCriterion_Integer::ClearEvaluationData()
{
	Min = 0;
	Max = 0;
	bEnableStrictMode = false;
}

void FFilterCriterion_Integer::CopyEvaluationData(const TInstancedStruct<FFilterCriterion>& Other)
{
	const FFilterCriterion_Integer* OtherCriterion = Other.GetPtr<FFilterCriterion_Integer>();
	if (OtherCriterion)
	{
		Min = OtherCriterion->Min;
		Max = OtherCriterion->Max;
		bEnableStrictMode = OtherCriterion->bEnableStrictMode;
	}
}

