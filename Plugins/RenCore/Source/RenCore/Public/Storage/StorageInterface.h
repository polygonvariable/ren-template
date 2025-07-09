// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers
#include "GameFramework/SaveGame.h"

// Generated Headers
#include "StorageInterface.generated.h"

// Forward Declarations



UINTERFACE(MinimalAPI, NotBlueprintable)
class UStorageSubsystemInterface : public UInterface
{
	GENERATED_BODY()
};

class RENCORE_API IStorageSubsystemInterface
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	virtual USaveGame* IGetLocalStorage() = 0;

};


