// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers
#include "RenCoreInventory/Public/InventoryContainer.h"

// Generated Headers
#include "InventoryProviderInterface.generated.h"

// Forward Declarations



UINTERFACE(MinimalAPI)
class UInventoryProviderInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class IInventoryProviderInterface
{

	GENERATED_BODY()

public:

	RENCOREINVENTORY_API virtual const TMap<FName, FInventoryContainer>& GetInventoryContainer() const = 0;
	RENCOREINVENTORY_API virtual TMap<FName, FInventoryContainer>& GetMutableInventoryContainer() = 0;

};

