// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InventoryWidget.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

// Project Headers
#include "RenAsset/Public/Inventory/InventoryAsset.h"

#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"

#include "RenInventory/Public/InventorySubsystem.h"



void UInventoryWidget::DisplayItems()
{
	if (!IsValid(InventorySubsystem))
	{
		LOG_ERROR(LogTemp, TEXT("InventorySubsystem is invalid"));
		return;
	}

	if (!EntryObjectClass || !EntryObjectClass->IsChildOf(UInventoryEntryObject::StaticClass()))
	{
		LOG_ERROR(LogTemp, TEXT("EntryObjectClass is invalid or not of type UInventoryEntryObject"));
		return;
	}

#if WITH_EDITOR

	PROFILE_START(Inventory);

#endif

	if (IsValid(InventoryContainer))
	{
		InventoryContainer->ClearListItems();
	}

	InventorySubsystem->QueryItems(FilterRule, QueryRule,
		[this](const FName& Guid, const FInventoryRecord* Record, UInventoryAsset* Asset)
		{
			UInventoryEntryObject* Entry = NewObject<UInventoryEntryObject>(this, EntryObjectClass);
			if (!IsValid(Entry))
			{
				LOG_ERROR(LogTemp, TEXT("Failed to create entry object"));
				return;
			}
			Entry->ItemGuid = Guid;
			Entry->InventoryAsset = Asset;
			Entry->InventoryRecord = (Record == nullptr) ? FInventoryRecord() : *Record;

			HandleDisplayOfEntry(Entry);
		}
	);

#if WITH_EDITOR

	PROFILE_END(Inventory, 5.0f, TEXT("Inventory rendered in"));

#endif
}

void UInventoryWidget::HandleDisplayOfEntry(UInventoryEntryObject* EntryObject)
{
	if (IsValid(InventoryContainer))
	{
		InventoryContainer->AddItem(EntryObject);
	}
}

void UInventoryWidget::HandleSelectedEntry(UObject* Object)
{
	UInventoryEntryObject* Entry = Cast<UInventoryEntryObject>(Object);
	if (!IsValid(Entry))
	{
		return;
	}
	OnInventoryItemSelected.Broadcast(Entry);
}


void UInventoryWidget::NativeConstruct()
{
	if (IsValid(InventoryContainer) && !InventoryContainer->OnItemSelectionChanged().IsBoundToObject(this))
	{
		InventoryContainer->OnItemSelectionChanged().AddUObject(this, &UInventoryWidget::HandleSelectedEntry);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UInventorySubsystem* InventorySubsystemPtr = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (!IsValid(InventorySubsystemPtr))
		{
			LOG_ERROR(LogTemp, TEXT("InventorySubsystem is invalid"));
			return;
		}

		InventorySubsystem = InventorySubsystemPtr;
	}

	Super::NativeConstruct();
}

void UInventoryWidget::NativeDestruct()
{
	if (IsValid(InventoryContainer))
	{
		InventoryContainer->OnItemSelectionChanged().RemoveAll(this);
	}

	InventorySubsystem = nullptr;

	Super::NativeDestruct();
}




void UInventoryEntryWidget::SelectInventoryEntry()
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
	if (!IsValid(Entry) && !Entry->InventoryAsset)
	{
		LOG_ERROR(LogTemp, "Entry or Asset is invalid");
		return;
	}
	UInventoryAsset* InventoryAsset = Entry->InventoryAsset;

	if (ItemTitleText) ItemTitleText->SetText(InventoryAsset->ItemName);
	if (ItemIconImage) ItemIconImage->SetBrushFromSoftTexture(InventoryAsset->ItemIcon);
	if (ItemQuantityText) ItemQuantityText->SetText(FText::AsNumber(Entry->InventoryRecord.ItemQuantity));

	HandleRecordValidity(Entry->InventoryRecord.IsValid());
}

void UInventoryEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (!IsValid(ListItemObject))
	{
		LOG_ERROR(LogTemp, "ListItemObject is invalid");
		return;
	}

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





void UInventoryDetailWidget::InitializeDetail_Implementation(FInventoryRecord Record, FName RecordId, UInventoryAsset* Asset)
{
	if (!IsValid(Asset))
	{
		if (IsValid(DetailSwitcher)) DetailSwitcher->SetActiveWidgetIndex(0);
		LOG_ERROR(LogTemp, "Asset is null");
		return;
	}

	//GET_GAMEINSTANCESUBSYSTEM_FROM_GAMEINSTANCE(UInventorySubsystem, InventorySubsystem);

	InventoryAsset = Asset;
	InventoryRecord = Record;
	InventoryRecordId = RecordId;

	HandleDetail();
}

void UInventoryDetailWidget::RefreshDetail_Implementation()
{
	if (!InventoryRecordId.IsValid() || !IsValid(InventorySubsystem)) {
		LOG_ERROR(LogTemp, "InventoryRecordId or InventorySubsystem is not valid");
		return;
	}

	/*InventoryRecord = InventorySubsystem->GetRecord(InventoryRecordId);
	if (!InventoryRecord.IsValid())
	{
		LOG_ERROR(LogTemp, "Record not found to refresh detail widget");
		return;
	}*/

	HandleDetail();
}

void UInventoryDetailWidget::HandleDetail_Implementation()
{
	if (IsValid(InventoryAsset))
	{
		if (IsValid(AssetTitle)) AssetTitle->SetText(InventoryAsset->ItemName);
		if (IsValid(AssetDescription)) AssetDescription->SetText(InventoryAsset->ItemDescription);
		if (IsValid(AssetImage) && InventoryAsset->ItemIcon.IsValid()) AssetImage->SetBrushFromSoftTexture(InventoryAsset->ItemIcon);

		if (IsValid(AssetTypeWidget))
		{
			AssetTypeWidget->SetVisibility(AssetTypeVisibility.Contains(InventoryAsset->ItemType) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}

		if (IsValid(RecordRank)) RecordRank->SetText(FText::FromString(FString::FromInt(InventoryRecord.EnhanceRecord.Rank)));
		if (IsValid(RecordLevel)) RecordLevel->SetText(FText::FromString(FString::FromInt(InventoryRecord.EnhanceRecord.Level)));
		if (IsValid(RecordExperience)) RecordExperience->SetText(FText::FromString(FString::FromInt(InventoryRecord.EnhanceRecord.Experience)));
	}

	if (IsValid(DetailSwitcher)) DetailSwitcher->SetActiveWidgetIndex(IsValid(InventoryAsset) ? 1 : 0);
}

