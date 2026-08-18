// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

// Project Headers
#include "Core/Type/EquipmentCategory.h"
#include "Core/Type/EquipmentTagData.h"
#include "Core/Type/Runtime/EquipmentInstance.h"

// Generated Headers
#include "EquipmentSettings.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class UEquipmentStorage;
class UEquipmentStorageManager;
class UEquipmentSubsystem;


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
	TMap<FGuid, FEquipmentInstance> DefaultEquipment;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Default Data")
	TMap<FGuid, FGuid> DefaultEquipmentRelations;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<UEquipmentSubsystem> SubsystemClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Asset")
	FPrimaryAssetType EquipmentType;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Asset")
	FPrimaryAssetType OwnerType;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Asset")
	TArray<FName> EquipmentBundles;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Tags", meta = (Categories = "Equipment"))
	FGameplayTagContainer EquipmentSlots;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Tags", meta = (Categories = "Equipment"))
	FGameplayTag EquipmentDataTag;


	UPROPERTY(Config, EditDefaultsOnly, Category = "Default Data")
	TArray<FEquipmentCategory> EquipmentCategory;


	/*
	 * DataTag -> <AbilityTag, EventTag, StateTag>
	 */
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Tags", meta = (Categories = "Data"))
	TMap<FGameplayTag, FEquipmentTagData> EquipmentTagRelations;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Gameplay Event")
	bool bAllowEventActivation = false;




	UPROPERTY(Config, EditDefaultsOnly, Category = "Notify Tags", meta = (Categories = "Event"))
	FGameplayTag EquipmentAttachNotify;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Notify Tags", meta = (Categories = "Event"))
	FGameplayTag EquipmentDetachNotify;


	static REN_API const FEquipmentSocket* GetSocketBySlotId(FGameplayTag CategoryTag, int SlotId, bool bEquip);
	static REN_API const FEquipmentSocket* GetSocketBySlotIndex(FGameplayTag CategoryTag, int SlotIndex, bool bEquip);


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Equipment Settings"))
	static REN_API const UEquipmentSettings* Get();


	static REN_API const FEquipmentTagData* GetTagData(const FGameplayTag& Tag);
	static REN_API const FEquipmentTagData* GetTagDataByAbility(const FGameplayTag& Tag);
	static REN_API const FGameplayTag& GetAttachmentEventTag(bool bEquip);

};


// Module Macros
#undef REN_API

