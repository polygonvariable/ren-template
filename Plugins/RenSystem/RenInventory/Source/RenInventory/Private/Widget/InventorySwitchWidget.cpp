// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventorySwitchWidget.h"

// Engine Headers
#include "Components/NamedSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/AssetManager.h"

// Project Headers
#include "RCoreInventory/Public/InventoryAsset.h"
#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/InventoryPrimaryAsset.h"



void UInventorySwitchWidget::InitializeDetails(const FPrimaryAssetId& AssetId, const FName& RecordId, const FInventoryRecord* Record)
{
	FAssetData AssetData;
	AssetManager->GetPrimaryAssetData(AssetId, AssetData);
	if (!IsValid(AssetManager))
	{
		ResetDetails();
		return;
	}

	EInventoryItemType ItemType = EInventoryItemType::Default;
	InventoryPrimaryAsset::GetItemType(AssetData, ItemType);

	EInventoryItemRarity ItemRarity = EInventoryItemRarity::Default;
	InventoryPrimaryAsset::GetItemRarity(AssetData, ItemRarity);
	
	if (FilterRule.Match(Record, RecordId, static_cast<uint8>(ItemType), static_cast<uint8>(ItemRarity)))
	{
		if (DetailSwitcher) DetailSwitcher->SetActiveWidgetIndex(0);
	}
	else
	{
		ResetDetails();
	}

	BP_HandleSwitch(AssetId, (Record ? *Record : FInventoryRecord()));
}

void UInventorySwitchWidget::ResetDetails()
{
	if (DetailSwitcher) DetailSwitcher->SetActiveWidgetIndex(1);
}

bool UInventorySwitchWidget::BP_HandleSwitch_Implementation(const FPrimaryAssetId& AssetId, FInventoryRecord Record)
{
	return false;
}

