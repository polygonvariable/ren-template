// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventorySwitchWidget.h"

// Engine Headers
#include "Components/NamedSlot.h"
#include "Components/WidgetSwitcher.h"

// Project Headers
#include "RenCoreInventory/Public/InventoryAsset.h"
#include "RenCoreInventory/Public/InventoryRecord.h"
#include "RenCore/Public/Macro/LogMacro.h"




void UInventorySwitchWidget::InitializeDetails(const FName& ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset)
{
	if (!Asset)
	{
		ResetDetails();
		LOG_ERROR(LogTemp, "Asset is invalid");
		return;
	}

	if (FilterRule.Match(Record, ItemGuid, Asset->ItemType, Asset->ItemRarity))
	{
		if (DetailSwitcher) DetailSwitcher->SetActiveWidgetIndex(0);
	}
	else
	{
		ResetDetails();
	}

	BP_HandleSwitch((Record ? *Record : FInventoryRecord()), Asset);
}

void UInventorySwitchWidget::ResetDetails()
{
	if (DetailSwitcher) DetailSwitcher->SetActiveWidgetIndex(1);
}

bool UInventorySwitchWidget::BP_HandleSwitch_Implementation(FInventoryRecord Record, UInventoryAsset* Asset)
{
	return false;
}

