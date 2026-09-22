// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Data/Asset/InstanceableDataAsset.h"

// Generated Headers
#include "CharacterAsset.generated.h"

// Forward Declarations
class UGameplayAbility;
class ACharacterBase;
class UCharacterTemplateAsset;


/**
 * 
 */
UCLASS(MinimalAPI)
class UCharacterAsset : public UInstanceableDataAsset
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


	UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "Character"))
	TSoftClassPtr<ACharacterBase> CharacterClass;

	UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "Ability"))
	TArray<TSoftClassPtr<UGameplayAbility>> CharacterAbilities;

	UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "Character"))
	TSoftObjectPtr<UCharacterTemplateAsset> CharacterTemplate;


	// ~ UPrimaryDataAsset
	RCHARACTER_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

public:

	static RCHARACTER_API FPrimaryAssetType GetPrimaryAssetType();

};


UCLASS(MinimalAPI)
class UEnemyCharacterAsset : public UCharacterAsset
{

	GENERATED_BODY()

};

