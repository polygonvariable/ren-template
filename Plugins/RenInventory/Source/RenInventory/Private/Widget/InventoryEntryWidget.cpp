// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryEntryWidget.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

// Project Headers
#include "RenAsset/Public/Inventory/InventoryAsset.h"

#include "RenCore/Public/Macro/LogMacro.h"

#include "RenInventory/Public/Widget/InventoryEntryObject.h"



void UInventoryEntryWidget::SelectEntry()
{
	UObject* ListItem = GetListItem();
	UListViewBase* ListViewBase = GetOwningListView();

	if (!IsValid(ListItem) || !IsValid(ListViewBase))
	{
		LOG_ERROR(LogTemp, "ListItem or ListViewBase is invalid");
		return;
	}

	UListView* ListView = Cast<UListView>(ListViewBase);
	if (!IsValid(ListView))
	{
		LOG_ERROR(LogTemp, "ListView is invalid");
		return;
	}

	const ESelectionMode::Type& SelectionMode = ListView->GetSelectionMode();
	switch (SelectionMode)
	{
		case ESelectionMode::None:
			// Deselect all ?
			// ListView->SetSelectedIndex(-1);
			// ListView->SetSelectedItem(nullptr);
			break;
		case ESelectionMode::Single:
			ListView->SetSelectedItem(ListItem);
			break;
		case ESelectionMode::SingleToggle:
			ListView->SetItemSelection(ListItem, !ListView->IsItemSelected(ListItem));
			break;
		case ESelectionMode::Multi:
			ListView->SetItemSelection(ListItem, !ListView->IsItemSelected(ListItem));
			break;
		default:
			break;
	}
}

void UInventoryEntryWidget::HandleInventoryEntry(UInventoryEntryObject* Entry)
{
	if (!IsValid(Entry) || !Entry->InventoryAsset)
	{
		LOG_ERROR(LogTemp, "Entry or Asset is invalid");
		return;
	}

	UInventoryAsset* InventoryAsset = Entry->InventoryAsset;
	const FInventoryRecord* InventoryRecord = Entry->InventoryRecord;
	bool bRecordIsValid = InventoryRecord && InventoryRecord->IsValid();

	if (ItemTitleText) ItemTitleText->SetText(InventoryAsset->ItemName);
	if (ItemIconImage) ItemIconImage->SetBrushFromSoftTexture(InventoryAsset->ItemIcon);
	if (ItemQuantityText) ItemQuantityText->SetText(FText::AsNumber(bRecordIsValid ? InventoryRecord->ItemQuantity : 0));
	
	HandleRecordValidity(bRecordIsValid);
}

void UInventoryEntryWidget::RemoveEntry()
{
	UObject* ListItem = GetListItem();
	UListViewBase* ListViewBase = GetOwningListView();

	IObjectPoolInterface* Entry = Cast<IObjectPoolInterface>(ListItem);
	if (Entry)
	{
		Entry->ReturnToPool();
	}

	UListView* ListView = Cast<UListView>(ListViewBase);
	if (ListView)
	{
		ListView->RemoveItem(ListItem);
	}
}

void UInventoryEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UInventoryEntryObject* Entry = Cast<UInventoryEntryObject>(ListItemObject);
	if (!IsValid(Entry))
	{
		LOG_ERROR(LogTemp, "Entry is invalid");
		return;
	}

	HandleInventoryEntry(Entry);
}

void UInventoryEntryWidget::NativeOnItemSelectionChanged(bool bSelected)
{
	HandleEntrySelectionChanged(bSelected);
}

