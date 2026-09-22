// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "PartySubsystem.generated.h"

// Forward Declarations
class IStorageProvider;
class UPartyStorageManager;


/**
 * 
 */
UCLASS(MinimalAPI)
class UPartySubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE(FOnSyncParty);
	FOnSyncParty OnPartyUpdated;


	RAVATARSPAWNER_API UPartyStorageManager* GetStorageManager();

	RAVATARSPAWNER_API bool TrySetPartyCharacter(int Slot, const FPrimaryAssetId& AssetId);
	RAVATARSPAWNER_API bool TryRemovePartyCharacter(int Slot);

protected:

	UPROPERTY()
	TObjectPtr<UPartyStorageManager> StorageManager = nullptr;

	IStorageProvider* StorageProvider = nullptr;


	// ~ Binding
	void HandleOnPreGameInitialized();
	// ~ End of Binding

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static RAVATARSPAWNER_API UPartySubsystem* Get(UWorld* World);
	static RAVATARSPAWNER_API UPartySubsystem* Get(UGameInstance* GameInstance);

};

