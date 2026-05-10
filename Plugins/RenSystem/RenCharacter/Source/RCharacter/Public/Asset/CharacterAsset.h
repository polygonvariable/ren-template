// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Asset/InstancedDataAsset.h"
#include "Interface/AscensionProvider.h"

// Generated Headers
#include "CharacterAsset.generated.h"

// Module Macros
#define REN_API RCHARACTER_API

// Forward Declarations
class UGameplayAbility;
class UAssetGroup;
class UAssetCollection;
class ACharacterBase;

struct FAscensionData;



/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class UCharacterAsset : public UInstancedDataAsset, public IAscensionProvider
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, AssetRegistrySearchable)
	int Health = 500;


	UPROPERTY(EditDefaultsOnly)
	int PhysicalDamage = 60;

	UPROPERTY(EditDefaultsOnly)
	int PhysicalDefense = 70;


	UPROPERTY(EditDefaultsOnly)
	int ElementalDamage = 40;

	UPROPERTY(EditDefaultsOnly)
	int ElementalDefense = 50;


	UPROPERTY(EditDefaultsOnly, Meta = (AssetBundles = "Character"))
	TSoftClassPtr<ACharacterBase> CharacterClass;

	UPROPERTY(EditDefaultsOnly, Meta = (AssetBundles = "Ability"))
	TArray<TSoftClassPtr<UGameplayAbility>> CharacterAbilities;


	// ~ IAscensionProviderInterface
	REN_API virtual int GetExperienceInterval(int Level) const override;
	REN_API virtual int GetLevelInterval(int Rank) const override;
	REN_API virtual int GetMaxLevel() const override;
	REN_API virtual int GetMaxRank() const override;
	REN_API virtual const UAssetCollection* GetExperienceAssets(const FAscensionData& Ascension) const override;
	REN_API virtual const UAssetCollection* GetRankAssets(const FAscensionData& Ascension) const override;
	// ~ End of IAscensionProviderInterface

	// ~ UPrimaryDataAsset
	REN_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

protected:

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int ExperienceInterval = 5000;

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int LevelInterval = 10;

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int MaxLevel = 100;

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1))
	int MaxRank = 100;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> LevelUpAssets = nullptr;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> RankUpAssets = nullptr;

public:

	REN_API static FPrimaryAssetType GetPrimaryAssetType();

};


UCLASS(MinimalAPI)
class UEnemyCharacterAsset : public UCharacterAsset
{

	GENERATED_BODY()

};

// Module Macros
#undef REN_API

