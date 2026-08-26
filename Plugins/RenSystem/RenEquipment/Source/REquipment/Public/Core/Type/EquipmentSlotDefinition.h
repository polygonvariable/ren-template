// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Generated Headers
#include "EquipmentSlotDefinition.generated.h"


/**
 * 
 */
USTRUCT()
struct FEquipmentSlotDefinition
{

	GENERATED_BODY()

public:

	FEquipmentSlotDefinition() {}
	FEquipmentSlotDefinition(FGameplayTag InCategoryTag, int InSlotId) : CategoryTag(InCategoryTag), SlotId(InSlotId) {};


	UPROPERTY(EditAnywhere, meta = (Categories = "Equipment.Category"))
	FGameplayTag CategoryTag;

	UPROPERTY(EditAnywhere)
	int SlotId = 10;
	

	void Reset()
	{
		CategoryTag = FGameplayTag::EmptyTag;
		SlotId = -1;
	}

	bool IsValid() const
	{
		return CategoryTag.IsValid() && SlotId > 0;
	}

	friend inline bool operator == (const FEquipmentSlotDefinition& A, const FEquipmentSlotDefinition& B)
	{
		return A.CategoryTag == B.CategoryTag && A.SlotId == B.SlotId;
	}

	friend inline uint32 GetTypeHash(const FEquipmentSlotDefinition& A)
	{
		uint32 Hash = GetTypeHash(A.CategoryTag);
		Hash = HashCombineFast(Hash, GetTypeHash(A.SlotId));
		return Hash;
	}

};

