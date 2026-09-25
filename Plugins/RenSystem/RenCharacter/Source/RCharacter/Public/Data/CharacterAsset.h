// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Data/Asset/InstanceableDataAsset.h"

// Generated Headers
#include "CharacterAsset.generated.h"

// Forward Declarations
class USkeletalMesh;
class UAnimBlueprint;
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

	UPROPERTY(EditDefaultsOnly, Category = "Presentation", meta = (AssetBundles = "Character"))
	TSoftObjectPtr<USkeletalMesh> SkeletonMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Presentation", meta = (AssetBundles = "Character"))
	TSoftObjectPtr<UAnimBlueprint> AnimBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Stats", AssetRegistrySearchable)
	int Health = 500;


	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int PhysicalDamage = 60;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int PhysicalDefense = 70;


	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int ElementalDamage = 40;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	int ElementalDefense = 50;


	UPROPERTY(EditDefaultsOnly, Category = "Gameplay", meta = (AssetBundles = "Character"))
	TSoftClassPtr<ACharacterBase> CharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay", meta = (AssetBundles = "Ability"))
	TArray<TSoftClassPtr<UGameplayAbility>> CharacterAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Template", meta = (AssetBundles = "Character"))
	TSoftObjectPtr<UCharacterTemplateAsset> CharacterTemplate;


	// ~ UPrimaryDataAsset
	RCHARACTER_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

#if WITH_EDITOR
	// ~ UPrimaryDataAsset
	RCHARACTER_API virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	// ~ End of UPrimaryDataAsset
#endif

public:

	static RCHARACTER_API FPrimaryAssetType GetPrimaryAssetType();

};


UCLASS(MinimalAPI)
class UEnemyCharacterAsset : public UCharacterAsset
{

	GENERATED_BODY()

};

