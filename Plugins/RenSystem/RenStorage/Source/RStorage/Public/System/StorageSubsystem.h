// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/StorageDefinition.h"
#include "Core/StorageProvider.h"

// Generated Headers
#include "StorageSubsystem.generated.h"

// Forward Declarations
class UStorageCache;


/**
 * 
 */
UCLASS()
class UStorageSubsystem : public UGameInstanceSubsystem, public IStorageProvider
{

	GENERATED_BODY()

public:

	// ~ IStorageProvider
	virtual UObject* GetStorageManager(const FName& StorageId) override;
	virtual void LoadStorage(const FStorageDefinition& Definition, FOnStorageLoaded&& Callback) override;
	virtual void SaveStorage(const FName& StorageId) override;
	// ~ End of IStorageProvider

protected:

	UPROPERTY()
	TObjectPtr<UStorageCache> StorageCache;

	UPROPERTY()
	TMap<FName, TObjectPtr<UObject>> ManagerCollection;


	void DeserializeStorage(const FName& StorageId, UObject* Storage, bool& bIsNewlyCreated);
	void SerializeStorage(const FName& StorageId, UObject* Storage);

	UObject* CreateStorageManager(const FStorageDefinition& Definition);

	void LoadStorageCache();
	void UnloadStorageCache();

	void UnloadStorage(const FName& StorageId);

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Object) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static UStorageSubsystem* Get(UWorld* World);
	static UStorageSubsystem* Get(UGameInstance* GameInstance);

};

