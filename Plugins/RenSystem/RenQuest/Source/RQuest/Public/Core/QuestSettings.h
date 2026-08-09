// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "QuestSettings.generated.h"

// Forward Declarations
class UQuestStorage;
class UQuestStorageManager;
class UQuestSubsystem;
class UQuestEngine;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Quest"))
class UQuestSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UQuestSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	FName StorageId = TEXT_EMPTY;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UQuestStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UQuestStorageManager> StorageManagerClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Asset Bundle")
	FName BundleName = TEXT("Quest");

	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<UQuestSubsystem> SubsystemClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Engine")
	TSubclassOf<UQuestEngine> QuestEngineClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Broadcast", meta = (Categories = "Event"))
	FGameplayTag EventTag_QuestBroadcast;

public:

	static const UQuestSettings* Get();

};

