// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Project Headers
#include "Definition/Runtime/EquipmentInstance.h"
#include "Interface/IStorageSettingsProvider.h"

// Generated Headers
#include "EquipmentSettings.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class UEquipmentStorage;
class UEquipmentSubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig)
class UEquipmentSettings : public UDeveloperSettings, public IStorageSettingsProvider
{

	GENERATED_BODY()

public:

	UEquipmentSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly)
	FName StorageId = TEXT_EMPTY;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UEquipmentStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UEquipmentSubsystem> SubsystemClass;

	UPROPERTY(Config, EditDefaultsOnly)
	TMap<FGuid, FEquipmentInstance> DefaultEquipment;

	UPROPERTY(Config, EditDefaultsOnly)
	FPrimaryAssetType EquipmentType;

	UPROPERTY(Config, EditDefaultsOnly)
	FPrimaryAssetType OwnerType;

	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTagContainer EquipmentSlots;

	UPROPERTY(Config, EditDefaultsOnly)
	TArray<FName> EquipmentBundles;

	//UPROPERTY(Config, EditDefaultsOnly, Meta = (MetaClass = "CoreDataAsset"))
	//TMap<FGameplayTag, FSoftClassPath> SupportedSlotClasses;


	// ~ IStorageSettingsProvider
	virtual const FName& GetStorageId() const override;
	virtual TSubclassOf<UStorage> GetStorageClass() const override;
	// ~ End of IStorageSettingsProvider

	REN_API static const UEquipmentSettings* Get();

};


// Module Macros
#undef REN_API

