// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/Type/EnhanceAsset.h"

// Engine Headers

// Project Headers
#include "RCoreEnhance/Public/EnhanceRecord.h"



float UEnhanceAsset::GetExchangedNumber(FInstancedStruct& Context) const
{
	const FEnhanceRecord* EnhanceRecord = Context.GetPtr<FEnhanceRecord>();
	if (!EnhanceRecord)
	{
		return ExchangePoints;
	}

	FString RowName = ExchangeCurve.RowName.ToString();
	int NewPoints = ExchangePoints;

	if (ExchangeCurve.IsValid(RowName))
	{
		NewPoints = ExchangeCurve.GetCurve(RowName)->Eval(EnhanceRecord->Level);
	}

	return FMath::Max(1, NewPoints);
}

