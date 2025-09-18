// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "InventoryManagerInterface.generated.h"

// Forward Declarations
class UInventoryAsset;



UINTERFACE(MinimalAPI)
class UInventoryManagerInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCOREINVENTORY_API IInventoryManagerInterface
{

	GENERATED_BODY()

public:

	virtual bool AddItem(FName ContainerId, UInventoryAsset* ItemAsset, int Quantity) = 0;
	virtual bool AddItems(FName ContainerId, const TMap<UInventoryAsset*, int>& ItemQuantities) = 0;

};

