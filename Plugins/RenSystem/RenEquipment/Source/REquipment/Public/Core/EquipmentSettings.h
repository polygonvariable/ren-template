// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

// Project Headers
#include "Core/Type/EquipmentCategoryDefinition.h"
#include "Core/Type/Runtime/EquipmentInstance.h"

// Generated Headers
#include "EquipmentSettings.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class UEquipmentStorage;
class UEquipmentStorageManager;
class UEquipmentSubsystem;
struct FEquipmentSlotId;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Equipment"))
class UEquipmentSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UEquipmentSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	FName StorageId = TEXT_EMPTY;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UEquipmentStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UEquipmentStorageManager> StorageManagerClass;


	UPROPERTY(Config, EditDefaultsOnly, Category = "Default Data")
	TMap<FGuid, FEquipmentOwnerInstance> DefaultEquipmentOwners;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Default Data")
	TMap<FGuid, FEquipmentSlotInstance> DefaultEquipmentInstances;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Default Data")
	TArray<FEquipmentCategoryDefinition> EquipmentCategories;


	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<UEquipmentSubsystem> SubsystemClass;


	UPROPERTY(Config, EditDefaultsOnly, Category = "Asset")
	FPrimaryAssetType EquipmentType;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Asset")
	FPrimaryAssetType OwnerType;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Asset")
	TArray<FName> EquipmentBundles;


	UPROPERTY(Config, EditDefaultsOnly, Category = "Tags", meta = (Categories = "Equipment.Cooldown"))
	FGameplayTag EquipmentCooldownTag;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Tags", meta = (Categories = "Equipment.Event"))
	FGameplayTag EquipmentAttachNotify;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Tags", meta = (Categories = "Equipment.Event"))
	FGameplayTag EquipmentDetachNotify;


	static REN_API const UEquipmentSettings* Get();

	static REN_API const FEquipmentCategoryDefinition* GetEquipmentCategoryByTag(FGameplayTag CategoryTag);
	static REN_API const FEquipmentSlotDefinition* GetEquipmentSlotById(FGameplayTag CategoryTag, int Id);
	static REN_API const FEquipmentSlotDefinition* GetEquipmentSlotById(const FEquipmentSlotId& SlotId);

	static REN_API const FGameplayTag& GetAttachmentEventTag(bool bEquip);

};


// Module Macros
#undef REN_API

