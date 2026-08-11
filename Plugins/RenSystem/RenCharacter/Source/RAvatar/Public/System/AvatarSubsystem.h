// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Interface/AssetInstanceCollectionProvider.h"

// Generated Headers
#include "AvatarSubsystem.generated.h"

// Module Macros
#define REN_API RAVATAR_API

// Forward Declarations
class IStorageProvider;
class UAvatarStorageManager;


/**
 * 
 */
UCLASS(MinimalAPI)
class UAvatarSubsystem : public UGameInstanceSubsystem, public IAssetInstanceCollectionProvider
{

	GENERATED_BODY()

public:

	REN_API UAvatarStorageManager* GetStorageManager() const;

	// ~ IAssetInstanceCollectionProvider
	virtual IAssetInstanceCollection* GetInstanceCollection(const FName& CollectionId) const override;
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

	static REN_API UAvatarSubsystem* Get(UWorld* World);
	static REN_API UAvatarSubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef REN_API

