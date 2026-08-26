// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "EquipmentAbilityCollection.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class FObjectPreSaveContext;
class UGameplayAbility;
class UGameplayEffect;


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

	UPROPERTY(EditAnywhere)
	bool bEnableInput = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bEnableInput==true", EditConditionHides))
	int InputId = -1;

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



// Module Macros
#undef REN_API

