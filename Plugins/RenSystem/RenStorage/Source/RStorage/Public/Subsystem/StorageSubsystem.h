// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Interfaces/IHttpRequest.h"

// Project Headers
#include "Definition/StorageHandle.h"
#include "Definition/TaskType.h"
#include "Interface/IStorageProvider.h"

// Generated Headers
#include "StorageSubsystem.generated.h"

// Forward Declarations
class FJsonObject;
class USaveGame;
class IStorageManager;


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
	virtual void LoadStorage(const FStorageDefinition& Definition, FTaskCallback&& Callback) override;
	virtual void SaveStorage(const FName& StorageId) override;
	// ~ End of IStorageProvider

protected:

	bool bRemoteLoad = false;
	bool bUseExperimental = false;
	
	UPROPERTY()
	TMap<FName, TObjectPtr<UObject>> StorageCollection;
	

	int GetSlotIndex();
	void SetSlotIndex(int Index);

	FString MakeUniqueStorageId(TSubclassOf<UObject> StorageClass, const FString& SlotName, int SlotIndex) const;
	void GetDefaultQuery(const FName& StorageId, TSharedPtr<FJsonObject>& QueryJson);
	void SerializeQuery(TSharedPtr<FJsonObject>& QueryJson, FString& OutString);

	TSubclassOf<USaveGame> GetStorageClass(const FStorageDefinition& Definition) const;
	TSubclassOf<UObject> GetStorageManagerClass(const FStorageDefinition& Definition) const;

	UObject* CreateStorageManager(const FStorageDefinition& Definition);
	USaveGame* CreateStorage(const FStorageDefinition& Definition, bool& bOutIsNew);
	USaveGame* CreateStorage(const FStorageDefinition& Definition);

	void LoadRemoteStorage(const FStorageDefinition& Definition, FTaskCallback&& Callback);
	void HandleRemoteStorageResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded, FStorageDefinition Definition, FTaskCallback Callback);

	void LoadLocalStorage(const FStorageDefinition& Definition, FTaskCallback&& Callback);
	bool SaveLocalStorage(USaveGame* SaveGame, const FName& StorageId);

	void UnloadAllStorages();
	void UnloadStorage(const FName& StorageId);

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

