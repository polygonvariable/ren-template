// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "RewardSettings.generated.h"

// Forward Declarations
class URewardStorage;
class URewardStorageManager;
class URewardSubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, Meta = (DisplayName = "RSystem - Reward"))
class URewardSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	URewardSettings(const FObjectInitializer& ObjectInitializer)
	{
		CategoryName = TEXT("Ren Project");
	}


	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Storage")
	FName StorageId;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<URewardStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<URewardStorageManager> StorageManagerClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<URewardSubsystem> SubsystemClass;


	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTag TagEvent_RewardInvoke;
	

	static const URewardSettings* Get()
	{
		return GetDefault<URewardSettings>();
	}

};

