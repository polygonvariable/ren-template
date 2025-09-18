// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryEntryWidget.h"

// Engine Headers
#include "Engine/AssetManager.h"

#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

// Project Headers
#include "RCoreInventory/Public/InventoryAsset.h"
#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/InventoryDefinition.h"
#include "RenInventory/Public/Widget/InventoryEntryObject.h"




void UInventoryEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UInventoryEntryObject* EntryObject = Cast<UInventoryEntryObject>(ListItemObject);
	if (!IsValid(EntryObject))
	{
		LOG_ERROR(LogTemp, TEXT("Entry is invalid"));
		return;
	}

	InitializeDetails(
		EntryObject->AssetId,
		EntryObject->RecordId,
		EntryObject->Record
	);
}

void UInventoryEntryWidget::InitializeDetails(const FPrimaryAssetId& AssetId, const FName& RecordId, const FInventoryRecord* Record)
{
	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, AssetId, [this](FPrimaryAssetId AssetId, UObject* LoadedAsset)
		{
			UInventoryAsset* InventoryAsset = Cast<UInventoryAsset>(LoadedAsset);
			if (IsValid(InventoryAsset))
			{
				SetPrimaryDetails(
					InventoryAsset->ItemName,
					InventoryAsset->ItemDescription,
					InventoryAsset->ItemIcon
				);
			}
		}
	);

	int Quantity = Record ? Record->ItemQuantity : 0;
	SetSecondaryDetails(FText::FromName(RecordId), Quantity);
}

void UInventoryEntryWidget::SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image)
{
	if (ItemTitleText) ItemTitleText->SetText(Title);
	if (ItemIconImage) ItemIconImage->SetBrushFromSoftTexture(Image);
}

void UInventoryEntryWidget::SetSecondaryDetails(const FText& RecordId, int Quantity)
{
	if (RecordIdText) RecordIdText->SetText(RecordId);
	if (ItemQuantityText) ItemQuantityText->SetText(FText::AsNumber(Quantity));
}

void UInventoryEntryWidget::SetTertiaryDetails(UInventoryEntryObject* Entry)
{
	HandleDetailsValidity((Entry && Entry->Record));
}

void UInventoryEntryWidget::ResetDetails()
{
	FText EmptyText = FText::GetEmpty();

	SetPrimaryDetails(EmptyText, EmptyText, nullptr);
	SetSecondaryDetails(EmptyText, 0);
}


void UInventoryEntryWidget::SelectEntry()
{
	UObject* ListItem = GetListItem();
	UListView* ListView = Cast<UListView>(GetOwningListView());

	if (!IsValid(ListItem) || !IsValid(ListView))
	{
		LOG_ERROR(LogTemp, TEXT("ListItem or ListView is invalid"));
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

void UInventoryEntryWidget::RemoveEntry()
{
	UObject* ListItem = GetListItem();
	UListViewBase* ListViewBase = GetOwningListView();

	UInventoryEntryObject* Entry = Cast<UInventoryEntryObject>(ListItem);
	if (Entry)
	{
		Entry->ResetData();
	}

	UListView* ListView = Cast<UListView>(ListViewBase);
	if (ListView)
	{
		ListView->RemoveItem(ListItem);
	}
}

void UInventoryEntryWidget::NativeOnItemSelectionChanged(bool bSelected)
{
	HandleSelectionChanged(bSelected);
}



void UWInventoryEntryQuantity::SetTertiaryDetails(UInventoryEntryObject* Entry)
{
	if (!Entry && !Entry->Record)
	{
		HandleDetailsValidity(false);
		return;
	}

	const FInventoryPayloadQuantity* Payload = Entry->Payload.GetPtr<FInventoryPayloadQuantity>();
	if (!Payload)
	{
		HandleDetailsValidity(false);
		return;
	}

	if (ItemRequiredText) ItemRequiredText->SetText(FText::FromString(FString::FromInt(Payload->Quantity)));

	bool bRecordValid = Payload->Quantity >= Entry->Record->ItemQuantity;
	HandleDetailsValidity(bRecordValid);
}


