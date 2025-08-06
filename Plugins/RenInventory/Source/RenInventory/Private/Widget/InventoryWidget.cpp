// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryWidget.h"

// Engine Headers

// Project Headers
#include "RenInventory/Public/Widget/InventoryCollectionWidget.h"
#include "RenInventory/Public/Widget/InventoryDetailWidget.h"



void UInventoryWidget::HandleItemSelected(FName Guid, const FInventoryRecord* Record, UInventoryAsset* Asset)
{
	if (InventoryDetail)
	{
		InventoryDetail->InitializeDetail(Guid, Record, Asset);
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

