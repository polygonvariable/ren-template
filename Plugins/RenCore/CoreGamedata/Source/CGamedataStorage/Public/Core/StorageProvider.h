// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers
#include "Core/Type/StorageDefinition.h"

// Generated Headers
#include "StorageProvider.generated.h"

// Delegate Declarations
DECLARE_DELEGATE_OneParam(FOnStorageLoaded, UObject* /* StorageManager */);


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UStorageProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CGAMEDATASTORAGE_API IStorageProvider
{

	GENERATED_BODY()

public:

	virtual UObject* GetStorageManager(const FName& StorageId) = 0;

	template<typename T>
	T* GetStorageManager(const FName& StorageId)
	{
		return Cast<T>(GetStorageManager(StorageId));
	}

	virtual void LoadStorage(const FStorageDefinition& Definition, FOnStorageLoaded&& Callback) = 0;
	virtual void SaveStorage(const FName& StorageId) = 0;

};


// Module Macros
#undef REN_API

