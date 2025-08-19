// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryEntryWidget.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

// Project Headers
#include "RenCoreInventory/Public/InventoryAsset.h"
#include "RenCoreInventory/Public/InventoryRecord.h"
#include "RenCoreLibrary/Public/LogMacro.h"

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
		EntryObject->ItemGuid,
		EntryObject->InventoryRecord,
		EntryObject->InventoryAsset
	);

	SetTertiaryDetails(EntryObject);
}



void UInventoryEntryWidget::InitializeDetails(const FName& ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset)
{
	if (!Asset)
	{
		LOG_ERROR(LogTemp, TEXT("Asset is invalid"));
		return;
	}

	SetPrimaryDetails(
		Asset->ItemName,
		Asset->ItemDescription,
		Asset->ItemIcon
	);
	SetSecondaryDetails(
		FText::FromName(ItemGuid),
		Record ? Record->ItemQuantity : 0
	);
}

void UInventoryEntryWidget::SetPrimaryDetails(const FText& Title, const FText& Description, TSoftObjectPtr<UTexture2D> Image)
{
	if (ItemTitleText) ItemTitleText->SetText(Title);
	if (ItemIconImage) ItemIconImage->SetBrushFromSoftTexture(Image);
}

void UInventoryEntryWidget::SetSecondaryDetails(const FText& Guid, int Quantity)
{
	if (ItemGuidText) ItemGuidText->SetText(Guid);
	if (ItemQuantityText) ItemQuantityText->SetText(FText::AsNumber(Quantity));
}

void UInventoryEntryWidget::SetTertiaryDetails(UInventoryEntryObject* Entry)
{
	HandleDetailsValidity((Entry && Entry->InventoryRecord));
}

void UInventoryEntryWidget::ResetDetails()
{
	FText Empty = FText::GetEmpty();

	SetPrimaryDetails(Empty, Empty, nullptr);
	SetSecondaryDetails(Empty, 0);
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

void UInventoryEntryWidget::NativeOnItemSelectionChanged(bool bSelected)
{
	HandleSelectionChanged(bSelected);
}



void UWInventoryEntryQuantity::SetTertiaryDetails(UInventoryEntryObject* Entry)
{
	if (!Entry && !Entry->InventoryRecord)
	{
		HandleDetailsValidity(false);
		return;
	}

	const FInventoryPayloadQuantity* Payload = Entry->InventoryPayload.GetPtr<FInventoryPayloadQuantity>();
	if (!Payload)
	{
		HandleDetailsValidity(false);
		return;
	}

	if (ItemRequiredText) ItemRequiredText->SetText(FText::FromString(FString::FromInt(Payload->Quantity)));

	bool bRecordValid = Payload->Quantity >= Entry->InventoryRecord->ItemQuantity;
	HandleDetailsValidity(bRecordValid);
}


