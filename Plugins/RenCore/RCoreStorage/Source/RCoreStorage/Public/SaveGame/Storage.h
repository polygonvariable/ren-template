// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/SaveGame.h"

// Project Headers
#include "Delegate/GameEventDelegate.h" 

// Generated Headers
#include "Storage.generated.h"

// Module Macros
#define REN_API RCORESTORAGE_API


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UStorage : public USaveGame
{

	GENERATED_BODY()

public:

	FGameEventDelegate OnStorageUpdated;

	REN_API virtual void InitializeDefaults();
	REN_API virtual void InitializeStorage();
	REN_API virtual void DeinitializeStorage();

	REN_API virtual void NetDeserialize(TSharedPtr<FJsonObject>& JsonObject);
	REN_API bool GetForceSave() const;

protected:

	UPROPERTY(Transient)
	bool bForceSave = true;

};


// Module Macros
#undef REN_API

