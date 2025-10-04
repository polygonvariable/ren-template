// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryViewUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/InventoryEntry.h"
#include "RenInventory/Public/Widget/InventoryUI.h"
#include "RenInventory/Public/Widget/InventoryCollectionUI.h"
#include "RenInventory/Public/Widget/InventoryDetailUI.h"
#include "RenInventory/Public/Widget/InventorySwitchUI.h"



void UInventoryViewUI::DisplayEnhanceItem()
{
	if (!InventoryCollection)
	{
		LOG_ERROR(LogInventory, TEXT("InventoryCollection is invalid"));
		return;
	}

	UInventoryEntry* Entry = InventoryCollection->GetSelectedItem();
	if (!IsValid(Entry))
	{
		LOG_ERROR(LogInventory, TEXT("Entry is invalid"));
		return;
	}

	if (IsValid(EnhanceItemWidget))
	{
		EnhanceItemWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UInventoryUI* Widget = CreateWidget<UInventoryUI>(this, EnhanceItemClass);
		if (!IsValid(Widget))
		{
			LOG_ERROR(LogInventory, TEXT("EnhanceItemWidget is invalid"));
			return;
		}

		Widget->AddToViewport();
		Widget->InitializeDetails(
			Entry->AssetId,
			Entry->Quantity,
			Entry->Record
		);

		EnhanceItemWidget = Widget;
	}
}

void UInventoryViewUI::SetContainerId(FName ContainerId)
{
	InventoryDetail->ContainerId = ContainerId;
	InventoryCollection->QueryRule.ContainerId = ContainerId;
}

void UInventoryViewUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	if (InventoryDetail)
	{
		InventoryDetail->InitializeDetails(AssetId, Quantity, Record);
	}

	if (InventorySwitch)
	{
		InventorySwitch->InitializeDetails(AssetId, Quantity, Record);
	}
}


void UInventoryViewUI::NativeConstruct()
{
	if (InventoryCollection)
	{
		InventoryCollection->OnItemSelected.AddUObject(this, &UInventoryViewUI::InitializeDetails);
	}

	UUserWidget::NativeConstruct();
}

void UInventoryViewUI::NativeDestruct()
{
	if (InventoryCollection)
	{
		InventoryCollection->OnItemSelected.RemoveAll(this);
	}

	UUserWidget::NativeDestruct();
}

