// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "ShopSettings.generated.h"

// Forward Declarations
class UShopStorage;
class UShopStorageManager;
class UShopSubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Trade Shop"))
class UShopSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UShopSettings(const FObjectInitializer& ObjectInitializer)
	{
		CategoryName = TEXT("Ren Project");
	}


	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	FName StorageId;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UShopStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UShopStorageManager> StorageManagerClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<UShopSubsystem> SubsystemClass;


	static const UShopSettings* Get()
	{
		return GetDefault<UShopSettings>();
	}

};

