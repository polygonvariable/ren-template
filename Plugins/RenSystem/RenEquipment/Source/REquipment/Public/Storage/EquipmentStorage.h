// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/Runtime/EquipmentInstance.h"
#include "SaveGame/Storage.h"

// Generated Headers
#include "EquipmentStorage.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class UAssetManager;
struct FGameplayTag;


/**
 *
 */
UCLASS(MinimalAPI)
class UEquipmentStorage : public UStorage
{

	GENERATED_BODY()

public:

	const TMap<FGameplayTag, FEquipmentKey>* GetOwnedEquipment(const FGuid& OwnerId) const;

	REN_API bool GetEquipmentAtSlot(const FGuid& InOwnerId, const FGameplayTag& InEquipmentSlot, FPrimaryAssetId& OutEquipmentAssetId) const;
	REN_API bool SetEquipmentAtSlot(const FGuid& OwnerId, const FPrimaryAssetId& OwnerAssetId, const FGameplayTag& EquipmentSlot, const FGuid& EquipmentId, const FPrimaryAssetId& EquipmentAssetId);
	REN_API bool RemoveEquipmentFromSlot(const FGuid& OwnerId, const FGameplayTag& SlotTag);

	// ~ UStorage
	virtual void InitializeDefaults();
	// ~ End of UStorage

protected:

	UPROPERTY(SaveGame)
	TMap<FGuid, FEquipmentInstance> EquipmentInstances; // OwnerId -> Equipment Instance

	UPROPERTY(SaveGame)
	TMap<FGuid, FGuid> EquipmentRelation; // EquipmentId -> OwnerId

};


// Module Macros
#undef REN_API

