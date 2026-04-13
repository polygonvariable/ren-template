// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Interfaces/IHttpRequest.h"

// Project Headers
#include "Definition/StorageHandle.h"
#include "Interface/IStorageProvider.h"

// Generated Headers
#include "StorageSubsystem.generated.h"

// Forward Declarations
class UStorage;
class UDeveloperSettings;
class FJsonObject;


/**
 * 
 */
UCLASS()
class UStorageSubsystem : public UGameInstanceSubsystem, public IStorageProvider
{

	GENERATED_BODY()

public:

	// ~ IStorageProvider
	virtual UStorage* GetStorage(const FName& StorageId) override;
	virtual void LoadStorage(FStorageHandle&& Handle) override;
	virtual void LoadStorageFromSettings(const UDeveloperSettings* Settings) override;
	virtual void SaveStorage(const FName& StorageId) override;
	// ~ End of IStorageProvider

protected:

	bool bNetLoad = false;

	UPROPERTY()
	TMap<FName, TObjectPtr<UStorage>> StorageCollection;


	int GetSlotIndex();
	void SetSlotIndex(int Index);


	bool MakeStorageId(TSubclassOf<UStorage> InStorageClass, const FString& InSlotName, int InSlotIndex, FString& OutStorageId) const;
	void GetDefaultQuery(const FName& StorageId, TSharedPtr<FJsonObject>& QueryJson);
	void SerializeQuery(TSharedPtr<FJsonObject>& QueryJson, FString& OutString);


	UStorage* CreateStorage_Internal(TSubclassOf<UStorage> StorageClass, const FName& StorageId);
	bool SaveStorage_Internal(UStorage* Storage, const FName& StorageId);


	void LoadStorage_Network(FStorageHandle Handle);
	void LoadStorage_NetworkResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded, FStorageHandle Handle);
	UStorage* LoadStorage_Internal(const FName& StorageId, TSubclassOf<UStorage> StorageClass);


	void OnPreGameInitialized();
	void SaveAllStorages();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Object) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

private:

	int _SlotIndex = 0;

public:

	static UStorageSubsystem* Get(UWorld* World);
	static UStorageSubsystem* Get(UGameInstance* GameInstance);

};

