// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Project Headers
#include "Definition/Runtime/InventoryStack.h"

// Generated Headers
#include "InventorySettings.generated.h"

// Forward Declarations
class UInventoryStorage;
class UInventoryStorageManager;
class UInventorySubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Inventory"))
class UInventorySettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UInventorySettings(const FObjectInitializer& ObjectInitializer)
	{
		CategoryName = TEXT("Ren Project");
	}


	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Storage")
	FName StorageId;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UInventoryStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UInventoryStorageManager> StorageManagerClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<UInventorySubsystem> SubsystemClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Default Data", meta = (AllowedTypes = "Asset.Inventory"))
	TMap<FPrimaryAssetId, FInventoryStack> DefaultInventory;


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Inventory Settings"))
	static const UInventorySettings* Get()
	{
		return GetDefault<UInventorySettings>();
	}

};

