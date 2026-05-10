// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Interface/AssetInstanceCollectionProvider.h"

// Generated Headers
#include "InventorySubsystem.generated.h"

// Module Macros
#define REN_API RINVENTORY_API

// Forward Declarations
class UInventoryStorageManager;
class IStorageProvider;


/**
 *
 *
 */
UCLASS(MinimalAPI)
class UInventorySubsystem : public UGameInstanceSubsystem, public IAssetInstanceCollectionProvider
{

	GENERATED_BODY()

public:

	REN_API UInventoryStorageManager* GetStorageManager(const FName& InventoryId) const;

	// ~ IAssetInstanceCollectionProvider
	virtual IAssetInstanceCollection* GetInstanceCollection(const FName& SourceId) const override;
	virtual FPrimaryAssetType GetSupportedAssetType() const override;
	virtual FName GetPrimaryCollectionId() const override;
	// ~ End of IAssetInstanceCollectionProvider

protected:

	IStorageProvider* StorageProvider;


	void HandlePreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static REN_API UInventorySubsystem* Get(UWorld* World);
	static REN_API UInventorySubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef REN_API

