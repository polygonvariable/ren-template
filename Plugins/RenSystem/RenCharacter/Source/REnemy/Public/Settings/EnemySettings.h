// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "InstancedStruct.h"

// Generated Headers
#include "EnemySettings.generated.h"

// Forward Declarations
class UEnemyStorage;
class UEnemyStorageManager;
class UEnemySubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, Meta = (DisplayName = "RSystem - Character Enemy"))
class UEnemySettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UEnemySettings(const FObjectInitializer& ObjectInitializer)
	{
		CategoryName = TEXT("Ren Project");
	}

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

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Meta = (DisplayName = "Enemy Died Tag"))
	FGameplayTag ETag_EnemyDied;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ETag_EnemyUpdateUI;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ETag_EnemyPartyDied;


	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Get Enemy Settings"))
	static const UEnemySettings* Get()
	{
		return GetDefault<UEnemySettings>();
	}

};

