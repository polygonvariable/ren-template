// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/AscensionFragment.h"

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Core/Type/AscensionData.h"
#include "Core/Type/AssetRuleDefinition.h"
#include "Data/AssetCollection.h"
#include "Data/AssetGroup.h"


int UAscensionFragment::GetExperienceInterval(int Level) const
{
	return ExperienceInterval;
}

int UAscensionFragment::GetLevelInterval(int Rank) const
{
	return LevelInterval;
}

int UAscensionFragment::GetMaxLevel() const
{
	return MaxLevel;
}

int UAscensionFragment::GetMaxRank() const
{
	return MaxRank;
}

const UAssetCollection* UAscensionFragment::GetExperienceAssets(const FAscensionData& Ascension) const
{
	if (!IsValid(LevelUpAssets))
	{
		return nullptr;
	}
	return LevelUpAssets->GetCollectionRule<UAssetCollection>();
}

const UAssetCollection* UAscensionFragment::GetRankAssets(const FAscensionData& Ascension) const
{
	if (!IsValid(RankUpAssets))
	{
		return nullptr;
	}

	FAssetRuleContext_List Context;
	Context.Index = Ascension.Rank;

	return RankUpAssets->GetCollectionRule<UAssetCollection>(FInstancedStruct::Make(Context));
}

