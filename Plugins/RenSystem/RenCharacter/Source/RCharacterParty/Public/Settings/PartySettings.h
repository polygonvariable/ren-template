// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Project Headers
#include "Interface/IStorageSettingsProvider.h"

// Generated Headers
#include "PartySettings.generated.h"

// Forward Declarations
class UPartyStorage;
class UPartySubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, Meta = (DisplayName = "RSystem - Character Party"))
class UPartySettings : public UDeveloperSettings, public IStorageSettingsProvider
{

	GENERATED_BODY()

public:

	UPartySettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Config, EditDefaultsOnly)
	FName StorageId = TEXT_EMPTY;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UPartyStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UPartySubsystem> SubsystemClass;

	UPROPERTY(Config, EditDefaultsOnly, Meta=(ClampMin=1, ClampMax=6))
	int MinPartySize = 1;

	UPROPERTY(Config, EditDefaultsOnly, Meta=(ClampMin=1, ClampMax=6))
	int MaxPartySize = 3;

	UPROPERTY(Config, EditDefaultsOnly)
	TArray<FPrimaryAssetId> DefaultCharacters;

	UPROPERTY(Config, EditDefaultsOnly)
	TArray<FName> CharacterBundles;


	// ~ IStorageSettingsProvider
	virtual const FName& GetStorageId() const override;
	virtual TSubclassOf<UStorage> GetStorageClass() const override;
	// ~ End of IStorageSettingsProvider

	static const UPartySettings* Get();

};

