// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Interface/AssetInstanceRelation.h"
#include "Core/Type/Runtime/EquipmentInstance.h"
#include "Delegate/GameEventDelegate.h"
#include "Interface/StorageManager.h"

// Generated Headers
#include "EquipmentStorageManager.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class UEquipmentStorage;
struct FGameplayTag;
struct FEquipmentInitializationData;


/**
 *
 */
UCLASS(MinimalAPI)
class UEquipmentStorageManager : public UObject, public IStorageManager, public IAssetInstanceRelation
{

	GENERATED_BODY()

public:

	FGameEventDelegate OnStorageUpdated;


	REN_API void GetEquipmentByOwnerId(const FGuid& InOwnerInstanceId, TArray<FEquipmentInitializationData>& OutInitializationData) const;
	REN_API void GetEquipmentIdsByOwnerId(const FGuid& InOwnerInstanceId, bool bInNegate, TArray<FGuid>& OutEquipmentInstanceIds) const;

	REN_API bool GetEquipmentAtSlot(const FGuid& InOwnerInstanceId, const FEquipmentSlotDefinition& InSlotDefinition, FPrimaryAssetId& OutEquipmentAssetId) const;
	REN_API bool SetEquipmentAtSlot(const FGuid& OwnerInstanceId, const FPrimaryAssetId& OwnerAssetId, const FGuid& EquipmentInstanceId, const FPrimaryAssetId& EquipmentAssetId, const FEquipmentSlotDefinition& SlotDefinition);
	REN_API bool RemoveEquipmentFromSlot(const FGuid& OwnerInstanceId, const FEquipmentSlotDefinition& SlotDefinition);

	// ~ IAssetInstanceRelation
	virtual bool HasLinkedInstance(const FPrimaryAssetId& AssetId, const FGuid& AssetInstanceId) const override;
	// ~ End of IAssetInstanceRelation

	// ~ UStorageManager
	virtual UObject* GetStorage() const override;
	virtual void SetStorage(UObject* Storage) override;
	virtual void OnStorageLoaded(bool bIsNew) override;
	virtual FGameEventDelegate& GetOnStorageUpdated() override;
	// ~ End of UStorageManager

protected:

	UPROPERTY()
	TObjectPtr<UEquipmentStorage> LocalStorage = nullptr;

};


// Module Macros
#undef REN_API

