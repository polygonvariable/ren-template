// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "EquipmentSubsystem.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class IStorageProvider;
class UEquipmentStorageManager;
struct FTaskResult;
struct FGameplayTag;


/**
 * 
 */
UCLASS(MinimalAPI)
class UEquipmentSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSyncEquipment, const FGuid& /* OwnerInstanceId */);
	FOnSyncEquipment OnSyncEquipment;

	REN_API void SyncEquipment(const FGuid& OwnerInstanceId) const;
	REN_API UEquipmentStorageManager* GetStorageManager() const;

	REN_API bool TrySetEquipmentSlot(const FGuid& OwnerInstanceId, const FPrimaryAssetId& OwnerAssetId, const FGameplayTag& Slot, const FGuid& EquipmentInstanceId, const FPrimaryAssetId& EquipmentAssetId);
	REN_API bool TryRemoveEquipmentSlot(const FGuid& OwnerInstanceId, const FGameplayTag& Slot);

protected:

	IStorageProvider* StorageProvider;

	void HandleStorageLoaded(const FTaskResult& Result);
	void HandleOnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static REN_API UEquipmentSubsystem* Get(UWorld* World);
	static REN_API UEquipmentSubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef REN_API

