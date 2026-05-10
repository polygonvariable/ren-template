// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Definition/TaskType.h"

// Generated Headers
#include "PartySubsystem.generated.h"

// Module Macros
#define REN_API RCHARACTERPARTY_API

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
	FOnSyncParty OnSyncParty;


	REN_API UPartyStorageManager* GetStorageManager();
	REN_API void SyncParty();

	REN_API bool TrySetPartyCharacter(int Slot, const FPrimaryAssetId& AssetId);
	REN_API bool TryRemovePartyCharacter(int Slot);

protected:

	UPROPERTY()
	TObjectPtr<UPartyStorageManager> StorageManager;

	IStorageProvider* StorageProvider;


	void HandlePreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static REN_API UPartySubsystem* Get(UWorld* World);
	static REN_API UPartySubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef REN_API

