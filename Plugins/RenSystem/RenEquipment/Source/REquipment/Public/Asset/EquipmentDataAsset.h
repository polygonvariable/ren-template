// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "EquipmentDataAsset.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class FObjectPreSaveContext;
class UGameplayAbility;
class UGameplayEffect;


/**
 *
 */
//USTRUCT()
//struct FEquipmentAbilityData
//{
//	
//	GENERATED_BODY()
//
//public:
//
//	UPROPERTY(EditAnywhere)
//	FGameplayTag EventTag;
//
//	UPROPERTY(EditAnywhere)
//	TArray<TSubclassOf<UGameplayAbility>> AbilityClasses;
//
//};

/**
 *
 */
UCLASS(MinimalAPI)
class UEquipmentAbilityCollection : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	//UPROPERTY(EditAnywhere)
	//TArray<FEquipmentAbilityData> Abilities;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGameplayAbility>> AbilityClasses;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGameplayEffect>> EffectClasses;


	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ UObject

};



// Module Macros
#undef REN_API

