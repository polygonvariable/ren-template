// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "FilterContext.h"

// Engine Headers

// Project Headers



bool FFilterContext::GetTypedValue(FName Key, FName& OutValue) const
{
	const TPair<FName, FName>* Found = TextProperties.FindByPredicate([Key](const TPair<FName, FName>& Pair) { return Pair.Key == Key; });
	if (Found)
	{
		OutValue = Found->Value;
		return true;
	}
	return false;
}

bool FFilterContext::GetTypedValue(FName Key, int& OutValue) const
{
	const TPair<FName, int>* Found = IntProperties.FindByPredicate([Key](const TPair<FName, int>& Pair) { return Pair.Key == Key; });
	if (Found)
	{
		OutValue = Found->Value;
		return true;
	}
	return false;
}

bool FFilterContext::GetTypedValue(FName Key, FPrimaryAssetId& OutValue) const
{
	const TPair<FName, FPrimaryAssetId>* Found = AssetProperties.FindByPredicate([Key](const TPair<FName, FPrimaryAssetId>& Pair) { return Pair.Key == Key; });
	if (Found)
	{
		OutValue = Found->Value;
		return true;
	}
	return false;
}

void FFilterContext::SetTypedValue(FName Key, FName Value)
{
	TextProperties.Add({ Key, Value });
}

void FFilterContext::SetTypedValue(FName Key, int Value)
{
	IntProperties.Add({ Key, Value });
}

void FFilterContext::SetTypedValue(FName Key, FPrimaryAssetId Value)
{
	AssetProperties.Add({ Key, Value });
}

