// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers
#include "RenCore/Public/Record/InventoryRecord.h"

// Generated Headers
#include "InventoryInterface.generated.h"

// Forward Declarations



UINTERFACE(MinimalAPI)
class UInventoryStorageInterface : public UInterface
{
	GENERATED_BODY()
};

class RENCORE_API IInventoryStorageInterface
{
	GENERATED_BODY()

public:

	virtual TMap<FName, FInventoryRecord>& GetInventory() = 0;

};

