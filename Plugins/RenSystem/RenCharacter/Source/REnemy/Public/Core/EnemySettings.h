// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "EnemySettings.generated.h"

// Module Macros
#define REN_API RENEMY_API

// Forward Declarations
class UEnemyStorage;
class UEnemyStorageManager;
class UEnemySubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Character Enemy"))
class UEnemySettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UEnemySettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	FName StorageId = TEXT_EMPTY;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UEnemyStorage> StorageClass = nullptr;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UEnemyStorageManager> StorageManagerClass = nullptr;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<UEnemySubsystem> SubsystemClass = nullptr;


	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTag TagEvent_DropReward;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "Enemy Died Tag"))
	FGameplayTag ETag_EnemyDied;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ETag_EnemyUpdateUI;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ETag_EnemyPartyDied;


	static REN_API const UEnemySettings* Get();

};


// Module Macros
#undef REN_API

