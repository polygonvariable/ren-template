// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Core/Type/EquipmentSocket.h"

// Generated Headers
#include "EquipmentCategory.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FEquipmentCategory
{

	GENERATED_BODY()

public:

	FEquipmentCategory() {};
	FEquipmentCategory(FGameplayTag InCategoryTag, int InSlotLimit)
	{
		CategoryTag = InCategoryTag;
		SlotLimit = InSlotLimit;
	};

	UPROPERTY(Config, EditDefaultsOnly, meta = (Categories = "Equipment.Category"))
	FGameplayTag CategoryTag;

	UPROPERTY(Config, EditDefaultsOnly, meta = (ClampMin = 1, ClampMax = 10))
	int SlotLimit = 1;

	UPROPERTY(EditAnywhere)
	TArray<FEquipmentSocket> EquipSockets;

	UPROPERTY(EditAnywhere)
	TArray<FEquipmentSocket> UnequipSockets;

};

