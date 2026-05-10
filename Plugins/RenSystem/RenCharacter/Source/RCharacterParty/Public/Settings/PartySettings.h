// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "PartySettings.generated.h"

// Forward Declarations
class UPartyStorage;
class UPartyStorageManager;
class UPartySubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, Meta = (DisplayName = "RSystem - Character Party"))
class UPartySettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UPartySettings(const FObjectInitializer& ObjectInitializer)
	{
		CategoryName = TEXT("Ren Project");
	}


	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	FName StorageId = TEXT_EMPTY;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UPartyStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Storage")
	TSubclassOf<UPartyStorageManager> StorageManagerClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<UPartySubsystem> SubsystemClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Default Data", Meta = (AllowedTypes = "Asset.Character"))
	TArray<FPrimaryAssetId> DefaultCharacters;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Party", Meta=(ClampMin=1, ClampMax=6))
	int MinPartySize = 1;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Party", Meta=(ClampMin=1, ClampMax=6))
	int MaxPartySize = 3;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Asset")
	TArray<FName> CharacterBundles;


	static const UPartySettings* Get()
	{
		return GetDefault<UPartySettings>();
	}

};

