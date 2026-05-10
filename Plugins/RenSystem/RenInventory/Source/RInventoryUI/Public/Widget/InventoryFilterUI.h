// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetFilterUI.h"

// Generated Headers
#include "InventoryFilterUI.generated.h"

// Forward Declarations
class UCoreDataAsset;


/**
 * 
 */
UCLASS(Abstract)
class UInventoryFilterUI : public UAssetFilterUI
{

	GENERATED_BODY()

protected:

	// ~ UAssetFilterUI
	// virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	// ~ End of UAssetFilterUI

};

