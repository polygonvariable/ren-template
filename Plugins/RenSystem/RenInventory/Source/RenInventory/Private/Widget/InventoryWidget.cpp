// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryWidget.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/Widget/InventoryBaseWidget.h"
#include "RenInventory/Public/Widget/InventoryCollectionWidget.h"
#include "RenInventory/Public/Widget/InventoryDetailWidget.h"
#include "RenInventory/Public/Widget/InventoryEntryObject.h"
#include "RenInventory/Public/Widget/InventorySwitchWidget.h"



void UInventoryWidget::DisplayEnhanceItem()
{
	if (!InventoryCollection)
	{
		LOG_ERROR(LogTemp, TEXT("InventoryCollection is invalid"));
		return;
	}

	UInventoryEntryObject* Entry = InventoryCollection->GetSelectedItem();
	if (!IsValid(Entry))
	{
		LOG_ERROR(LogTemp, TEXT("Entry is invalid"));
		return;
	}

	if (IsValid(EnhanceItemWidget))
	{
		EnhanceItemWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UInventoryBaseWidget* Widget = CreateWidget<UInventoryBaseWidget>(this, EnhanceItemClass);
		if (!IsValid(Widget))
		{
			LOG_ERROR(LogTemp, TEXT("EnhanceItemWidget is invalid"));
			return;
		}
		Widget->AddToViewport();
		// Widget->InitializeDetails(Entry->ItemGuid, Entry->InventoryRecord, Entry->InventoryAsset);

		EnhanceItemWidget = Widget;
	}
}

void UInventoryWidget::SetContainerId(FName ContainerId)
{
	InventoryDetail->ContainerId = ContainerId;
	InventoryCollection->QueryRule.ContainerId = ContainerId;
}

void UInventoryWidget::HandleItemSelected(const FPrimaryAssetId& AssetId, FName RecordId, const FInventoryRecord* Record)
{
	if (InventoryDetail)
	{
		InventoryDetail->InitializeDetails(AssetId, RecordId, Record);
	}
	if (InventorySwitch)
	{
		InventorySwitch->InitializeDetails(AssetId, RecordId, Record);
	}
}

void UInventoryWidget::NativeConstruct()
{
	if (InventoryCollection)
	{
		InventoryCollection->OnItemSelected.AddUObject(this, &UInventoryWidget::HandleItemSelected);
	}

	Super::NativeConstruct();
}

void UInventoryWidget::NativeDestruct()
{
	if (InventoryCollection)
	{
		InventoryCollection->OnItemSelected.RemoveAll(this);
	}

	Super::NativeDestruct();
}

