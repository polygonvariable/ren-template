// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Core/Type/EquipmentSlotDefinition.h"

// Generated Headers
#include "EquipmentCategoryDefinition.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FEquipmentCategoryDefinition
{

	GENERATED_BODY()

public:

	FEquipmentCategoryDefinition() {}
	FEquipmentCategoryDefinition(FGameplayTag InCategoryTag, TArray<FEquipmentSlotDefinition> InSlots) : CategoryTag(InCategoryTag), Slots(MoveTemp(InSlots)) {};


	UPROPERTY(EditAnywhere, meta = (Categories = "Equipment.Category"))
	FGameplayTag CategoryTag;

	UPROPERTY(EditAnywhere)
	TArray<FEquipmentSlotDefinition> Slots;


	bool IsValid() const
	{
		return CategoryTag.IsValid();
	}

};

