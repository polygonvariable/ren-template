// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "CraftSettings.generated.h"

// Forward Declarations
class UCraftStorage;
class UCraftStorageManager;
class UCraftSubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Trade Craft"))
class UCraftSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UCraftSettings(const FObjectInitializer& ObjectInitializer)
	{
		CategoryName = TEXT("Ren Project");
	}


	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	FName StorageId;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UCraftStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UCraftStorageManager> StorageManagerClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<UCraftSubsystem> SubsystemClass;


	static const UCraftSettings* Get()
	{
		return GetDefault<UCraftSettings>();
	}

};

