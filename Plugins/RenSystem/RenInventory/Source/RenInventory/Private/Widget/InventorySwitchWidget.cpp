// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventorySwitchWidget.h"

// Engine Headers
#include "Components/NamedSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/AssetManager.h"

// Project Headers
#include "RCoreFilter/Public/FilterContext.h"
#include "RCoreFilter/Public/FilterCriterion.h"
#include "RCoreFilter/Public/FilterGroup.h"

#include "RCoreInventory/Public/InventoryRecord.h"

#include "RenInventory/Public/InventoryPrimaryAsset.h"



void UInventorySwitchWidget::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	if (!IsValid(AssetManager))
	{
		ResetDetails();
		return;
	}

	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
	{
		ResetDetails();
		return;
	}

	FName ItemType = TEXT_EMPTY;
	InventoryPrimaryAsset::GetItemType(AssetData, ItemType);

	FName ItemRarity = TEXT_EMPTY;
	InventoryPrimaryAsset::GetItemRarity(AssetData, ItemRarity);

	FFilterContext Context;
	Context.SetValue(InventoryFilterProperty::AssetId, AssetId);
	Context.SetValue(InventoryFilterProperty::AssetType, ItemType);
	Context.SetValue(InventoryFilterProperty::AssetRarity, ItemRarity);
	
	if (FilterRule->CriterionRoot->Evaluate(Context))
	{
		if (DetailSwitcher)
		{
			DetailSwitcher->SetActiveWidgetIndex(0);
		}
	}
	else
	{
		ResetDetails();
	}

	FInventoryRecord LocalRecord = Record ? *Record : FInventoryRecord();
	BP_HandleSwitch(AssetId, LocalRecord);
}

void UInventorySwitchWidget::ResetDetails()
{
	if (DetailSwitcher)
	{
		DetailSwitcher->SetActiveWidgetIndex(1);
	}
}

bool UInventorySwitchWidget::BP_HandleSwitch_Implementation(const FPrimaryAssetId& AssetId, FInventoryRecord Record)
{
	return false;
}

