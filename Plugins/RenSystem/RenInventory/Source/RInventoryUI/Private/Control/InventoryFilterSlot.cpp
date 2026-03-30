// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Control/InventoryFilterSlot.h"

// Project Headers
#include "Asset/InventoryAsset.h"
#include "Definition/AssetFilterProperty.h"
#include "Definition/FilterContext.h"
#include "Definition/InventoryFilterProperty.h"
#include "Widget/InventoryEntry.h"



void UInventoryFilterSlot::InitializeAssetDetail(const UCoreDataAsset* Asset)
{
	const UInventoryAsset* InventoryAsset = Cast<UInventoryAsset>(Asset);
	if (!InventoryAsset)
	{
		return;
	}

	const UEnum* EnumItemType = StaticEnum<EInventoryItemType>();
	const UEnum* EnumItemRarity = StaticEnum<EInventoryItemRarity>();

	FFilterContext Context;
	Context.SetValue(FAssetFilterProperty::AssetId, InventoryAsset->GetPrimaryAssetId());
	Context.SetValue(FInventoryFilterProperty::ItemType, *EnumItemType->GetNameStringByValue((int64)InventoryAsset->Type));
	Context.SetValue(FInventoryFilterProperty::ItemRarity, *EnumItemRarity->GetNameStringByValue((int64)InventoryAsset->Rarity));

	Evaluate(Context);
}

void UInventoryFilterSlot::InitializeEntryDetail(const UAssetEntry* Entry)
{
	const UInventoryEntry* InventoryEntry = Cast<UInventoryEntry>(Entry);
	if (!InventoryEntry)
	{
		return;
	}

	FFilterContext Context;
	Context.SetValue(FInventoryFilterProperty::ItemId, FName(*Entry->GetAssetInstanceId().ToString()));
	Context.SetValue(FInventoryFilterProperty::ItemQuantity, InventoryEntry->Quantity);

	Evaluate(Context);
}
