// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Core/Type/EquipmentSlotData.h"

// Generated Headers
#include "EquipmentCategoryData.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FEquipmentCategoryData
{

	GENERATED_BODY()

public:

	FEquipmentCategoryData() {}
	FEquipmentCategoryData(FGameplayTag InCategoryTag, TArray<FEquipmentSlotData> InSlots) : CategoryTag(InCategoryTag), Slots(MoveTemp(InSlots)) {};


	UPROPERTY(EditAnywhere, meta = (Categories = "Equipment.Category"))
	FGameplayTag CategoryTag;

	UPROPERTY(EditAnywhere)
	TArray<FEquipmentSlotData> Slots;


	bool IsValid() const
	{
		return CategoryTag.IsValid();
	}

};

