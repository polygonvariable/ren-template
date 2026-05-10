// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryEntryUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Widget/InventoryEntry.h"


void UInventoryEntryUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	if (!IsValid(Asset))
	{
		return;
	}

	EntryName->SetText(Asset->DisplayName);
	EntryIcon->SetBrushFromSoftTexture(Asset->Icon);

	SetToolTipText(Asset->DisplayName);
}

void UInventoryEntryUI::SetSecondaryDetail(const UAssetEntry* Entry)
{
	const UInventoryEntry* InventoryEntry = Cast<UInventoryEntry>(Entry);
	if (IsValid(InventoryEntry))
	{
		ItemQuantity->SetText(FText::AsNumber(InventoryEntry->Quantity));
		ItemRelation->SetVisibility(InventoryEntry->bIsLinked ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}

