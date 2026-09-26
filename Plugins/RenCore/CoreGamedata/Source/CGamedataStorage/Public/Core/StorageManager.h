// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "StorageManager.generated.h"

// Delegate Declarations
DECLARE_MULTICAST_DELEGATE(FOnStorageUpdated);


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UStorageManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CGAMEDATASTORAGE_API IStorageManager
{

	GENERATED_BODY()

public:

	virtual UObject* GetStorage() const = 0;

	template<typename T>
	T* GetStorage() const
	{
		return Cast<T>(GetStorage());
	}

	virtual void SetStorage(UObject* InStorage) = 0;

	virtual void OnStorageLoaded(bool bIsNew) {};
	virtual void OnBeginUnload() {};
	virtual void OnAfterUnload() {};

	FOnStorageUpdated& GetOnStorageUpdated()
	{
		return OnStorageUpdated;
	}

protected:

	FOnStorageUpdated OnStorageUpdated;

};

