// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventorySwitchUI.h"

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



void UInventorySwitchUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity)
{
	if (!IsValid(AssetManager) || !IsValid(FilterRule) || !IsValid(FilterRule->CriterionRoot))
	{
		SwitchDetails(false);
		return;
	}

	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
	{
		SwitchDetails(false);
		return;
	}

	FName ItemType = TEXT_EMPTY;
	InventoryPrimaryAsset::GetType(AssetData, ItemType);

	FName ItemRarity = TEXT_EMPTY;
	InventoryPrimaryAsset::GetRarity(AssetData, ItemRarity);

	FFilterContext Context;
	Context.SetValue(InventoryFilterProperty::AssetId, AssetId);
	Context.SetValue(InventoryFilterProperty::AssetType, ItemType);
	Context.SetValue(InventoryFilterProperty::AssetRarity, ItemRarity);

	bool bPrimary = FilterRule->CriterionRoot->Evaluate(Context);
	SwitchDetails(bPrimary);
}

void UInventorySwitchUI::SwitchDetails(bool bPrimary)
{
	ESlateVisibility NewVisibility = (bPrimary) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	SetVisibility(NewVisibility);
}

