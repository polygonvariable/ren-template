// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/AssetEntry.h"

// Generated Headers
#include "InventoryEntry.generated.h"

// Forward Declarations
struct FInventoryInstance;


/**
 *
 */
UCLASS(MinimalAPI)
class UInventoryEntry : public UAssetEntry
{

	GENERATED_BODY()

public:

	int Quantity = 0;
	const FInventoryInstance* Item = nullptr;
	bool bIsLinked = false;

	// ~ UAssetEntry
	virtual FGuid GetAssetInstanceId() const override;
	virtual void ResetData() override;
	// ~ End of UAssetEntry

};

