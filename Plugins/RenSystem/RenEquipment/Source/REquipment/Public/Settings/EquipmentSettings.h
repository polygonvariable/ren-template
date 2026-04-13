// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

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
USTRUCT(BlueprintType)
struct FEquipmentTagRelation
{

	GENERATED_BODY()

public:

	/*
	 * Dynamic tag granted to ability, used to identify ability for activation.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AbilityTag;

	/*
	 * Tag that responds to event, can be used to activate ability.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EventTag;

	/*
	 * Tag granted to actor, can be used to identify state of ability or actor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag StateTag;

};


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, Meta = (DisplayName = "RSystem - Equipment"))
class UEquipmentSettings : public UDeveloperSettings, public IStorageSettingsProvider
{

	GENERATED_BODY()

public:

	UEquipmentSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	FName StorageId = TEXT_EMPTY;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UEquipmentStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TMap<FGuid, FEquipmentInstance> DefaultEquipment;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TMap<FGuid, FGuid> DefaultEquipmentRelations;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<UEquipmentSubsystem> SubsystemClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Asset")
	FPrimaryAssetType EquipmentType;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Asset")
	FPrimaryAssetType OwnerType;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Asset")
	TArray<FName> EquipmentBundles;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer EquipmentSlots;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Tags")
	FGameplayTag EquipmentDataTag;

	/*
	 * DataTag -> <AbilityTag, EventTag, StateTag>
	 */
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Tags")
	TMap<FGameplayTag, FEquipmentTagRelation> EquipmentTagRelations; 

	UPROPERTY(Config, EditDefaultsOnly, Category = "Gameplay Event")
	bool bAllowEventActivation = false;


	// ~ IStorageSettingsProvider
	virtual const FName& GetStorageId() const override;
	virtual TSubclassOf<UStorage> GetStorageClass() const override;
	// ~ End of IStorageSettingsProvider

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Get Equipment Settings"))
	static REN_API const UEquipmentSettings* Get();

};


// Module Macros
#undef REN_API

