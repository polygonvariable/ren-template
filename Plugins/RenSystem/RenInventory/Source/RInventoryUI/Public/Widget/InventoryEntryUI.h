// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetEntryUI.h"

// Generated Headers
#include "InventoryEntryUI.generated.h"

// Forward Declarations
class UCoreDataAsset;



/**
 *
 * 
 * 
 */
UCLASS(Abstract)
class UInventoryEntryUI : public UAssetEntryUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity = nullptr;


	// ~ UInventoryUI
	virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	// ~ End of UInventoryUI

};

