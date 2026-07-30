// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/Category/EnhanceableAsset.h"

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Definition/AscensionData.h"
#include "Definition/AssetRuleDefinition.h"
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"



int UEnhanceableAsset::GetExperienceInterval(int Level) const
{
	return ExperienceInterval;
}

int UEnhanceableAsset::GetLevelInterval(int Rank) const
{
	return LevelInterval;
}

int UEnhanceableAsset::GetMaxLevel() const
{
	return MaxLevel;
}

int UEnhanceableAsset::GetMaxRank() const
{
	return MaxRank;
}

const UAssetCollection* UEnhanceableAsset::GetExperienceAssets(const FAscensionData& Ascension) const
{
	if (!IsValid(ExperienceItems))
	{
		return nullptr;
	}
	return ExperienceItems->GetCollectionRule<UAssetCollection>();
}

const UAssetCollection* UEnhanceableAsset::GetRankAssets(const FAscensionData& Ascension) const
{
	if (!IsValid(RankItems))
	{
		return nullptr;
	}

	FAssetRuleContext_List Context;
	Context.Index = Ascension.Rank;

	return RankItems->GetCollectionRule<UAssetCollection>(FInstancedStruct::Make(Context));
}

