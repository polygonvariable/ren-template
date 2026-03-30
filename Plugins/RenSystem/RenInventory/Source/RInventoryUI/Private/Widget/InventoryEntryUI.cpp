// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryEntryUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/InventoryAsset.h"
#include "Asset/CoreDataAsset.h"
#include "Widget/InventoryEntry.h"



void UInventoryEntryUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	const UInventoryAsset* InventoryAsset = Cast<UInventoryAsset>(Asset);
	if (IsValid(InventoryAsset))
	{
		EntryName->SetText(InventoryAsset->DisplayName);
		EntryIcon->SetBrushFromSoftTexture(InventoryAsset->Icon);
	}
}

void UInventoryEntryUI::SetSecondaryDetail(const UAssetEntry* Entry)
{
	const UInventoryEntry* InventoryEntry = Cast<UInventoryEntry>(Entry);
	if (IsValid(InventoryEntry))
	{
		ItemQuantity->SetText(FText::AsNumber(InventoryEntry->Quantity));
	}
}

