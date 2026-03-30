// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers
#include "Definition/StorageHandle.h"

// Generated Headers
#include "IStorageProvider.generated.h"

// Module Macros
#define REN_API RCORESTORAGE_API

// Forward Declarations
class UStorage;
class UDeveloperSettings;



UINTERFACE(MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
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

	virtual UStorage* GetStorage(const FName& StorageId) = 0;

	template<typename T>
	T* GetStorage(const FName& StorageId)
	{
		return Cast<T>(GetStorage(StorageId));
	}

	virtual void LoadStorage(FStorageHandle&& Handle) = 0;
	virtual void LoadStorageFromSettings(const UDeveloperSettings* Settings) = 0;
	virtual void SaveStorage(const FName& StorageId) = 0;

public:

	static IStorageProvider* Get(UWorld* World);
	static IStorageProvider* Get(UGameInstance* GameInstance);

};



// Module Macros
#undef REN_API

