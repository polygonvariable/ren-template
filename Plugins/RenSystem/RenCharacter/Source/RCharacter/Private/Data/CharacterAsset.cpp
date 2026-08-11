// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/CharacterAsset.h"

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Definition/AscensionData.h"
#include "Definition/AssetRuleDefinition.h"
#include "Management/AssetCollection.h"
#include "Management/AssetGroup.h"



FPrimaryAssetId UCharacterAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UCharacterAsset::GetPrimaryAssetType(), GetFName());
}

FPrimaryAssetType UCharacterAsset::GetPrimaryAssetType()
{
	return TEXT("Asset.Character");
}



int UCharacterAsset::GetExperienceInterval(int Level) const
{
	return ExperienceInterval;
}

int UCharacterAsset::GetLevelInterval(int Rank) const
{
	return LevelInterval;
}

int UCharacterAsset::GetMaxLevel() const
{
	return MaxLevel;
}

int UCharacterAsset::GetMaxRank() const
{
	return MaxRank;
}

const UAssetCollection* UCharacterAsset::GetExperienceAssets(const FAscensionData& Ascension) const
{
	if (!IsValid(LevelUpAssets))
	{
		return nullptr;
	}
	return LevelUpAssets->GetCollectionRule<UAssetCollection>();
}

const UAssetCollection* UCharacterAsset::GetRankAssets(const FAscensionData& Ascension) const
{
	if (!IsValid(RankUpAssets))
	{
		return nullptr;
	}

	FAssetRuleContext_List Context;
	Context.Index = Ascension.Rank;

	return RankUpAssets->GetCollectionRule<UAssetCollection>(FInstancedStruct::Make(Context));
}


