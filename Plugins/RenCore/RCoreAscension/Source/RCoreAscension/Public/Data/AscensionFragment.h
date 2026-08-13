// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Data/AssetFragment.h"

// Generated Headers
#include "AscensionFragment.generated.h"

// Module Macros
#define REN_API RCOREASCENSION_API

// Forward Declarations
class UAssetGroup;
class UAssetCollection;
struct FAscensionData;


/**
 *
 */
UCLASS(Const)
class UAscensionFragment : public UAssetFragment
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1))
	int ExperienceInterval = 5000;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1))
	int LevelInterval = 10;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1))
	int MaxLevel = 100;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1))
	int MaxRank = 100;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> LevelUpAssets = nullptr;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> RankUpAssets = nullptr;


	REN_API virtual int GetExperienceInterval(int Level) const;
	REN_API virtual int GetLevelInterval(int Rank) const;
	REN_API virtual int GetMaxLevel() const;
	REN_API virtual int GetMaxRank() const;
	REN_API virtual const UAssetCollection* GetExperienceAssets(const FAscensionData& Ascension) const;
	REN_API virtual const UAssetCollection* GetRankAssets(const FAscensionData& Ascension) const;

};


// Module Macros
#undef REN_API

