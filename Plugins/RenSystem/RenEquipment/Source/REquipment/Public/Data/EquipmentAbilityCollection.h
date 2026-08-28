// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Generated Headers
#include "EquipmentAbilityCollection.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class UGameplayAbility;
class UGameplayEffect;
class UInputAction;


/**
 *
 */
USTRUCT()
struct FEquipmentAbilityData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 10))
	int InputIdOffset = 0;

};

/**
 *
 */
UCLASS(MinimalAPI)
class UEquipmentAbilityCollection : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FEquipmentAbilityData> Abilities;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGameplayEffect>> EffectClasses;

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ UObject

};

