// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Project Headers
#include "Definition/Runtime/InventoryStack.h"
#include "Interface/IStorageSettingsProvider.h"

// Generated Headers
#include "InventorySettings.generated.h"

// Forward Declarations
class UInventoryStorage;
class UInventorySubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, Meta = (DisplayName = "RSystem - Inventory"))
class UInventorySettings : public UDeveloperSettings, public IStorageSettingsProvider
{

	GENERATED_BODY()

public:

	UInventorySettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	FName StorageId;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UInventoryStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UInventorySubsystem> SubsystemClass;

	UPROPERTY(Config, EditDefaultsOnly, Meta = (AllowedTypes = "Asset.Inventory"))
	TMap<FPrimaryAssetId, FInventoryStack> DefaultInventory;


	// ~ IStorageSettingsProvider
	virtual const FName& GetStorageId() const override;
	virtual TSubclassOf<UStorage> GetStorageClass() const override;
	// ~ End of IStorageSettingsProvider

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Get Inventory Settings"))
	static const UInventorySettings* Get();

};

