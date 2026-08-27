// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Core/Type/EquipmentKey.h"
#include "Core/Type/EquipmentSlotId.h"

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


/**
 *
 */
USTRUCT()
struct FEquipmentOwnerInstance
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, SaveGame)
	FPrimaryAssetId OwnerAssetId;

	UPROPERTY(EditAnywhere, SaveGame)
	TMap<FEquipmentSlotId, FGuid> Slots;

};

/**
 *
 */
USTRUCT()
struct FEquipmentSlotInstance
{

	GENERATED_BODY()

public:

	FEquipmentSlotInstance() {};
	FEquipmentSlotInstance(FPrimaryAssetId InEquipmentAssetId, FGuid InOwnerInstanceId) : EquipmentAssetId(InEquipmentAssetId), OwnerInstanceId(InOwnerInstanceId) {};

	UPROPERTY(EditAnywhere, SaveGame)
	FPrimaryAssetId EquipmentAssetId;

	UPROPERTY(EditAnywhere, SaveGame)
	FGuid OwnerInstanceId;

};

