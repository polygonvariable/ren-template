// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Asset/Category/CraftableAsset.h"
#include "Interface/AscensionProvider.h"

// Generated Headers
#include "EnhanceableAsset.generated.h"

// Module Macros
#define REN_API RINVENTORY_API

// Forward Declarations
class UAssetGroup;
class UAssetCollection;

struct FAscensionData;



/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UEnhanceableAsset : public UCraftableAsset, public IAscensionProvider
{

	GENERATED_BODY()

public:

	// ~ IAscensionProvider
	REN_API virtual int GetExperienceInterval(int Level) const override;
	REN_API virtual int GetLevelInterval(int Rank) const override;
	REN_API virtual int GetMaxLevel() const override;
	REN_API virtual int GetMaxRank() const override;
	REN_API virtual const UAssetCollection* GetExperienceAssets(const FAscensionData& Ascension) const override;
	REN_API virtual const UAssetCollection* GetRankAssets(const FAscensionData& Ascension) const override;
	// ~ End of IAscensionProvider

protected:

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1))
	int ExperienceInterval = 5000;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1))
	int LevelInterval = 10;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1))
	int MaxLevel = 100;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1))
	int MaxRank = 100;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> ExperienceItems = nullptr;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> RankItems = nullptr;

};



// Module Macros
#undef REN_API

