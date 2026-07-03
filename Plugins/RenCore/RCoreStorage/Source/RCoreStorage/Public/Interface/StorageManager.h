// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Plugin Headers
#include "Delegate/GameEventDelegate.h"

// Generated Headers
#include "StorageManager.generated.h"

// Module Macros
#define REN_API RCORESTORAGE_API


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UStorageManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REN_API IStorageManager
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

	virtual void LoadRemoteData(TSharedPtr<FJsonObject>& JsonObject) {};
	virtual void OnStorageLoaded(bool bIsNew) {};

	virtual void OnBeginUnload() {};
	virtual void OnAfterUnload() {};

	virtual FGameEventDelegate& GetOnStorageUpdated() = 0;

};


// Module Macros
#undef REN_API

