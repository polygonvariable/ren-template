// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers
#include "GameFramework/SaveGame.h"

// Generated Headers
#include "StorageProviderInterface.generated.h"

// Forward Declarations



UINTERFACE(MinimalAPI, NotBlueprintable)
class UStorageProviderInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 * 
 */
class RCORESTORAGE_API IStorageProviderInterface
{

	GENERATED_BODY()

public:

	virtual USaveGame* GetLocalStorage() = 0;

};


