// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Core/Type/EquipmentKey.h"

// Generated Headers
#include "EquipmentInstance.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FEquipmentInstance
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, SaveGame)
	FPrimaryAssetId OwnerAssetId;

	UPROPERTY(EditAnywhere, SaveGame)
	TMap<FGameplayTag, FEquipmentKey> EquipmentSlot;

};

