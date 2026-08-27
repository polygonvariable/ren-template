// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Generated Headers
#include "EquipmentSlotId.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FEquipmentSlotId
{

	GENERATED_BODY()

public:

	FEquipmentSlotId() {}
	FEquipmentSlotId(FGameplayTag InSlotTag, int InId) : SlotTag(InSlotTag), Id(InId) {};


	UPROPERTY(EditAnywhere, meta = (Categories = "Equipment.Category"))
	FGameplayTag SlotTag;

	UPROPERTY(EditAnywhere)
	int Id = 10;
	

	void Reset()
	{
		SlotTag = FGameplayTag::EmptyTag;
		Id = -1;
	}

	bool IsValid() const
	{
		return SlotTag.IsValid() && Id > 0;
	}

	friend inline bool operator == (const FEquipmentSlotId& A, const FEquipmentSlotId& B)
	{
		return A.SlotTag == B.SlotTag && A.Id == B.Id;
	}

	friend inline uint32 GetTypeHash(const FEquipmentSlotId& A)
	{
		uint32 Hash = GetTypeHash(A.SlotTag);
		Hash = HashCombineFast(Hash, GetTypeHash(A.Id));
		return Hash;
	}

};

