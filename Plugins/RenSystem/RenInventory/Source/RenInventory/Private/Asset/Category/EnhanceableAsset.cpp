// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/Category/EnhanceableAsset.h"

// Engine Headers

// Project Headers



int UEnhanceableAsset::GetCurveValue(const FCurveTableRowHandle& Curve, int Default, int Time) const
{
	FString Context = Curve.RowName.ToString();
	int Value = Curve.IsValid(Context) ? Curve.GetCurve(Context)->Eval(Time) : Default;
	return FMath::Max(1, Value);
}

int UEnhanceableAsset::GetExperienceInterval(int Level) const
{
	return GetCurveValue(ExperienceCurve, ExperienceInterval, Level);
}

int UEnhanceableAsset::GetLevelInterval(int Rank) const
{
	return GetCurveValue(LevelCurve, LevelInterval, Rank);
}

int UEnhanceableAsset::GetMaxLevel() const
{
	return MaxLevel;
}

int UEnhanceableAsset::GetMaxRank() const
{
	return MaxRank;
}

const FExchangeRule& UEnhanceableAsset::GetEnhanceRules() const
{
	return EnhanceRules;
}

const TArray<FExchangeRule>& UEnhanceableAsset::GetRankingRules() const
{
	return RankingRules;
}

