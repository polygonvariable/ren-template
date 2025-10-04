// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryEntryUI.h"

// Engine Headers
#include "Engine/AssetManager.h"

#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

// Project Headers
#include "RCoreInventory/Public/InventoryAsset.h"
#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/InventoryDefinition.h"
#include "RenInventory/Public/InventoryEntry.h"




void UInventoryEntryUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UInventoryEntry* EntryObject = Cast<UInventoryEntry>(ListItemObject);
	if (!IsValid(EntryObject))
	{
		LOG_ERROR(LogInventory, TEXT("Entry is invalid"));
		return;
	}

	InitializeDetails(
		EntryObject->AssetId,
		EntryObject->Quantity,
		EntryObject->Record
	);
	SetTertiaryDetails(EntryObject);
}

void UInventoryEntryUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	TWeakObjectPtr<UInventoryEntryUI> WeakThis(this);
	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis, Quantity](bool, UObject* LoadedAsset)
		{
			UInventoryEntryUI* Owner = WeakThis.Get();
			UInventoryAsset* InventoryAsset = Cast<UInventoryAsset>(LoadedAsset);
			if (IsValid(Owner) && IsValid(InventoryAsset))
			{
				Owner->SetPrimaryDetails(
					InventoryAsset->DisplayName,
					InventoryAsset->Description,
					InventoryAsset->Icon
				);
				Owner->SetSecondaryDetails(Quantity);
			}
		};

	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, AssetId, MoveTemp(AsyncCallback));
}

void UInventoryEntryUI::SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image)
{
	if (ItemName) ItemName->SetText(Title);
	if (ItemIcon) ItemIcon->SetBrushFromSoftTexture(Image);
}

void UInventoryEntryUI::SetSecondaryDetails(int Quantity)
{
	if (ItemQuantity) ItemQuantity->SetText(FText::AsNumber(Quantity));
}

void UInventoryEntryUI::SetTertiaryDetails(UInventoryEntry* Entry)
{
	HandleDetailsValidity((Entry && Entry->Record));
}

void UInventoryEntryUI::ResetDetails()
{
	FText EmptyText = FText::GetEmpty();

	SetPrimaryDetails(EmptyText, EmptyText, nullptr);
	SetSecondaryDetails(0);
}


void UInventoryEntryUI::SelectEntry()
{
	UObject* ListItem = GetListItem();
	UListView* ListView = Cast<UListView>(GetOwningListView());

	if (!IsValid(ListItem) || !IsValid(ListView))
	{
		LOG_ERROR(LogInventory, TEXT("ListItem or ListView is invalid"));
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

void UInventoryEntryUI::RemoveEntry()
{
	UObject* ListItem = GetListItem();
	UListViewBase* ListViewBase = GetOwningListView();

	UInventoryEntry* Entry = Cast<UInventoryEntry>(ListItem);
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

void UInventoryEntryUI::NativeOnItemSelectionChanged(bool bSelected)
{
	HandleSelectionChanged(bSelected);
}



void UInventoryQuantityUI::SetTertiaryDetails(UInventoryEntry* Entry)
{
	if (!Entry)
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

	if (ItemAmount) ItemAmount->SetText(FText::FromString(FString::FromInt(Payload->Quantity)));

	bool bValid = Entry->Quantity >= Payload->Quantity;
	HandleDetailsValidity(bValid);
}


