// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Interface/AssetInstanceCollection.h"
#include "Interface/StorageManager.h"

// Generated Headers
#include "RewardStorageManager.generated.h"

// Module Macros
#define REN_API RREWARD_API

// Forward Declarations
class UAssetManager;
class URewardStorage;


/**
 *
 */
UCLASS(MinimalAPI)
class URewardStorageManager : public UObject, public IStorageManager, public IAssetInstanceCollection
{

	GENERATED_BODY()

public:

	// ~ IAssetInstanceCollection
	virtual bool AddInstance(const FPrimaryAssetId& AssetId, int Quantity) override;
	virtual bool RemoveInstance(const FPrimaryAssetId& AssetId, int Quantity) override;
	virtual bool ContainInstance(const FPrimaryAssetId& AssetId, int Quantity) const override;
	
	virtual FGameEventDelegate& GetOnAssetInstanceCollectionUpdated() override;
	// ~ End of IAssetInstanceCollection

	// ~ UStorageManager
	virtual UObject* GetStorage() const override;
	virtual void SetStorage(UObject* Storage) override;
	virtual FGameEventDelegate& GetOnStorageUpdated() override;
	// ~ End of UStorageManager

protected:

	FGameEventDelegate OnStorageUpdated;

private:

	UPROPERTY()
	TObjectPtr<URewardStorage> LocalStorage = nullptr;

};


// Module Macros
#undef REN_API

