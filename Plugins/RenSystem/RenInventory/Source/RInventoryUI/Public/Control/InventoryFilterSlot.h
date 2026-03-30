// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Control/AssetFilterSlot.h"

// Generated Headers
#include "InventoryFilterSlot.generated.h"

// Forward Declarations
class UCoreDataAsset;
class UAssetEntry;



/**
 *
 * 
 */
UCLASS()
class UInventoryFilterSlot : public UAssetFilterSlot
{

	GENERATED_BODY()

public:

	// ~ IAssetWidget
	virtual void InitializeAssetDetail(const UCoreDataAsset* Asset) override;
	virtual void InitializeEntryDetail(const UAssetEntry* Entry) override;
	// ~ End of IAssetWidget

};

