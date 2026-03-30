// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "PartySubsystem.generated.h"

// Module Macros
#define REN_API RCHARACTERPARTY_API

// Forward Declarations
class IStorageProvider;
class UPartyStorage;


/**
 * 
 * 
 * 
 */
UCLASS(MinimalAPI)
class UPartySubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE(FOnSyncParty);
	FOnSyncParty OnSyncParty;


	REN_API UPartyStorage* GetPartyStorage();
	REN_API void SyncParty();

protected:

	TWeakInterfacePtr<IStorageProvider> StorageProvider;


	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

private:

	UPROPERTY()
	TObjectPtr<UPartyStorage> _CachedStorage;

public:

	static REN_API UPartySubsystem* Get(UWorld* World);
	static REN_API UPartySubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef REN_API

