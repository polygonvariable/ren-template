// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/AssetEntryUI.h"

// Generated Headers
#include "InventoryEntryUI.generated.h"

// Forward Declarations
class UOverlay;
class UCoreDataAsset;


/**
 * 
 */
UCLASS(Abstract)
class UInventoryEntryUI : public UAssetEntryUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> ItemRelation = nullptr;

	// ~ UInventoryUI
	virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	// ~ End of UInventoryUI

};

