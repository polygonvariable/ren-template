// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetEntryUI.h"

// Generated Headers
#include "TradeEntryUI.generated.h"

// Forward Declarations
class UTextBlock;
class UAssetEntry;
class UCoreDataAsset;



/**
 *
 */
UCLASS(Abstract)
class UTradeEntryUI : public UAssetEntryUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> TradeQuota = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> TradeQuantity = nullptr;


	// ~ UInventoryUI
	virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	// ~ End of UInventoryUI

};

