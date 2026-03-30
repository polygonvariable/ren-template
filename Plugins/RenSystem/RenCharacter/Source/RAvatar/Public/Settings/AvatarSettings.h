// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Project Headers
#include "Definition/Runtime/AvatarInstance.h"
#include "Interface/IStorageSettingsProvider.h"

// Generated Headers
#include "AvatarSettings.generated.h"

// Module Macros
#define REN_API RAVATAR_API

// Forward Declarations
class UAvatarStorage;
class UAvatarSubsystem;



/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig)
class UAvatarSettings : public UDeveloperSettings, public IStorageSettingsProvider
{

	GENERATED_BODY()

public:

	UAvatarSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Config, EditDefaultsOnly)
	FName StorageId = TEXT_EMPTY;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UAvatarStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UAvatarSubsystem> SubsystemClass;

	UPROPERTY(Config, EditDefaultsOnly)
	TMap<FPrimaryAssetId, FAvatarInstance> DefaultAvatars;


	// ~ IStorageSettingsProvider
	virtual const FName& GetStorageId() const override;
	virtual TSubclassOf<UStorage> GetStorageClass() const override;
	// ~ End of IStorageSettingsProvider

	REN_API static const UAvatarSettings* Get();

};


// Module Macros
#undef REN_API

