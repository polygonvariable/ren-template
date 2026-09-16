// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/SaveGame.h"

// Project Headers
#include "Core/Type/StorageInstance.h"

// Generated Headers
#include "StorageCache.generated.h"


/*
 * 
 */
UCLASS()
class UStorageCache : public USaveGame
{

	GENERATED_BODY()

public:

	UPROPERTY(SaveGame)
	TArray<FStorageInstance> Instances;

};

