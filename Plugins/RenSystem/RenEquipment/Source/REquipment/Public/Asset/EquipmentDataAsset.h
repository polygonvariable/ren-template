// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Engine/DataAsset.h"
#include "Asset/MetadataAsset.h"

// Generated Headers
#include "EquipmentDataAsset.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API


/**
 *
 */
UCLASS(MinimalAPI)
class UEquipmentAbilityCollection : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGameplayAbility>> AbilityClasses;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGameplayEffect>> EffectClasses;

};



// Module Macros
#undef REN_API

