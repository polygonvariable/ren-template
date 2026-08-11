// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Project Headers
#include "Core/Type/Runtime/AvatarInstance.h"

// Generated Headers
#include "AvatarSettings.generated.h"

// Forward Declarations
class UAvatarStorage;
class UAvatarStorageManager;
class UAvatarSubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Character Avatar"))
class UAvatarSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UAvatarSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	FName StorageId = TEXT_EMPTY;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UAvatarStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UAvatarStorageManager> StorageManagerClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<UAvatarSubsystem> SubsystemClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Default Data", meta = (AllowedTypes = "Asset.Character"))
	TMap<FPrimaryAssetId, FAvatarInstance> DefaultAvatars;


	static const UAvatarSettings* Get();

};

