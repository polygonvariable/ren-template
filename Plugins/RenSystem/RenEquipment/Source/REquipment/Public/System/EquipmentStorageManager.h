// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/Runtime/EquipmentInstance.h"
#include "Delegate/GameEventDelegate.h"
#include "Core/Interface/AssetInstanceRelation.h"
#include "Interface/StorageManager.h"

// Generated Headers
#include "EquipmentStorageManager.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class UEquipmentStorage;
struct FGameplayTag;


/**
 *
 */
UCLASS(MinimalAPI)
class UEquipmentStorageManager : public UObject, public IStorageManager, public IAssetInstanceRelation
{

	GENERATED_BODY()

public:

	FGameEventDelegate OnStorageUpdated;

	REN_API const TMap<FGameplayTag, FEquipmentKey>* GetOwnedEquipment(const FGuid& OwnerId) const;
	REN_API void GetOwnedEquipmentIds(const FGuid& OwnerId, TArray<FGuid>& OutEquipmentIds) const;
	REN_API void GetNonOwnedEquipmentIds(const FGuid& OwnerId, TArray<FGuid>& OutEquipmentIds) const;

	REN_API bool GetEquipmentAtSlot(const FGuid& InOwnerId, const FGameplayTag& InEquipmentSlot, FPrimaryAssetId& OutEquipmentAssetId) const;
	REN_API bool SetEquipmentAtSlot(const FGuid& OwnerId, const FPrimaryAssetId& OwnerAssetId, const FGameplayTag& EquipmentSlot, const FGuid& EquipmentId, const FPrimaryAssetId& EquipmentAssetId);
	REN_API bool RemoveEquipmentFromSlot(const FGuid& OwnerId, const FGameplayTag& SlotTag);

	// ~ UStorageManager
	virtual UObject* GetStorage() const override;
	virtual void SetStorage(UObject* Storage) override;
	virtual void OnStorageLoaded(bool bIsNew) override;
	virtual FGameEventDelegate& GetOnStorageUpdated() override;
	// ~ End of UStorageManager

	// ~ IAssetInstanceRelation
	virtual bool HasLinkedInstance(const FPrimaryAssetId& AssetId, const FGuid& AssetInstanceId) const override;
	// ~ End of IAssetInstanceRelation

protected:

	UPROPERTY()
	TObjectPtr<UEquipmentStorage> LocalStorage = nullptr;

};


// Module Macros
#undef REN_API

