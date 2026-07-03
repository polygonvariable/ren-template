// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers
#include "Definition/StorageHandle.h"
#include "Definition/TaskType.h"

// Generated Headers
#include "StorageProvider.generated.h"

// Module Macros
#define REN_API RCORESTORAGE_API

// Forward Declarations
class IStorageManager;


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UStorageProvider : public UInterface
{

	GENERATED_BODY()

};

/**
 * 
 */
class REN_API IStorageProvider
{

	GENERATED_BODY()

public:

	virtual UObject* GetStorageManager(const FName& StorageId) = 0;

	template<typename T>
	T* GetStorageManager(const FName& StorageId)
	{
		return Cast<T>(GetStorageManager(StorageId));
	}

	virtual void LoadStorage(const FStorageDefinition& Definition, FTaskCallback&& Callback) = 0;
	virtual void SaveStorage(const FName& StorageId) = 0;

	//virtual void RegisterTransientStorage(const FName& StorageId, UObject* StorageManager, UObject* Storage) = 0;
	//virtual void UnregisterTransientStorage(const FName& StorageId) = 0;

public:

	static IStorageProvider* Get(UWorld* World);
	static IStorageProvider* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef REN_API

