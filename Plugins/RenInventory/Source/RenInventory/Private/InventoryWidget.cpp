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
#include "RenCore/Public/Macro/GameInstanceMacro.h"
#include "RenCore/Public/Macro/LogMacro.h"

// Module Headers
#include "InventorySubsystem.h"
#include "RenAsset/Public/Inventory/InventoryAsset.h"


void UInventoryWidget::DisplayStoredRecords_Implementation(const bool bForceRefresh)
{
	if (!EntryObjectClass || !EntryObjectClass->IsChildOf(UInventoryEntryObject::StaticClass()))
	{
		LOG_ERROR(LogTemp, "EntryObjectClass is null or not a child of UInventoryEntryObject");
		return;
	}

	GET_GAMEINSTANCESUBSYSTEM_FROM_GAMEINSTANCE(UInventorySubsystem, InventorySubsystem);

	if (InventoryRecords.IsEmpty() || bForceRefresh)
	{
		InventoryRecords = InventorySubsystem->GetRecords();
	}

	if (IsValid(InventoryContainer))
	{
		InventoryContainer->ClearListItems();
	}

	for (const auto& Record : InventoryRecords)
	{

		/*UInventoryAsset* Asset;
		if (CachedAssets.Contains(Record.Value.ItemId))
		{
			Asset = *CachedAssets.Find(Record.Value.ItemId);
		}
		else
		{
			Asset = InventorySubsystem->GetRecordAsset(Record.Value.ItemId);
			CachedAssets.Add(Record.Value.ItemId, Asset);
		}*/

		UInventoryAsset* Asset = InventorySubsystem->GetRecordAsset(Record.Value.ItemId);
		if (!IsValid(Asset))
		{
			LOG_ERROR(LogTemp, "Failed to get record's asset");
			continue;
		}

		if (!HandleEntryFiltering(Record.Value, Asset))
		{
			continue;
		}

		TObjectPtr<UInventoryEntryObject> EntryObject = NewObject<UInventoryEntryObject>(this, EntryObjectClass);
		if (!IsValid(EntryObject))
		{
			LOG_ERROR(LogTemp, "Failed to create entry object");
			continue;
		}
		EntryObject->InventoryRecordId = Record.Key;
		EntryObject->InventoryRecord = Record.Value;
		EntryObject->InventoryAsset = Asset;

		HandleDisplayOfEntry(EntryObject);
	}

}

void UInventoryWidget::HandleDisplayOfEntry_Implementation(UInventoryEntryObject* EntryObject)
{
	if (IsValid(InventoryContainer))
	{
		InventoryContainer->AddItem(EntryObject);
	}
}


bool UInventoryWidget::HandleEntryFiltering_Implementation(const FInventoryRecord InventoryRecord, UInventoryAsset* InventoryAsset)
{
	return false;
}


void UInventoryWidget::HandleSelectedEntry_Implementation(UInventoryEntryObject* EntryObject)
{

}

void UInventoryWidget::HandleSelectedEntryCast(UObject* Object)
{
	if (!IsValid(Object))
	{
		return;
	}
	UInventoryEntryObject* EntryObject = Cast<UInventoryEntryObject>(Object);
	HandleSelectedEntry(EntryObject);
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(IsValid(InventoryContainer) && !InventoryContainer->OnItemSelectionChanged().IsBound())
	{
		InventoryContainer->OnItemSelectionChanged().AddUObject(this, &UInventoryWidget::HandleSelectedEntryCast);
	}
}

void UInventoryWidget::NativeDestruct()
{
	if (IsValid(InventoryContainer) && InventoryContainer->OnItemSelectionChanged().IsBound())
	{
		InventoryContainer->OnItemSelectionChanged().RemoveAll(this);
	}
	Super::NativeDestruct();
}


void UInventoryEntryWidget::SelectEntry_Implementation()
{
	UListViewBase* ListViewBase = GetOwningListView();
	UObject* ListRecord = GetListItem();

	if (!IsValid(ListRecord) || !IsValid(ListViewBase))
	{
		LOG_ERROR(LogTemp, "ListRecord or ListViewBase is null");
		return;
	}

	UListView* ListView = Cast<UListView>(ListViewBase);
	ListView->SetSelectedItem(ListRecord);
}

void UInventoryEntryWidget::HandleEntry_Implementation(UInventoryEntryObject* EntryObject)
{
	if (!IsValid(InventoryAsset) && !IsValid(EntryObject))
	{
		LOG_ERROR(LogTemp, "InventoryAsset is null");
		return;
	}
	if(IsValid(AssetImage)) AssetImage->SetBrushFromSoftTexture(InventoryAsset->AssetIcon);
	if(IsValid(AssetTitle)) AssetTitle->SetText(InventoryAsset->AssetName);
}

void UInventoryEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (!IsValid(ListItemObject))
	{
		return;
	}
	UInventoryEntryObject* EntryObject = Cast<UInventoryEntryObject>(ListItemObject);
	InventoryRecordId = EntryObject->InventoryRecordId;
	InventoryRecord = EntryObject->InventoryRecord;
	InventoryAsset = EntryObject->InventoryAsset;

	HandleEntry(EntryObject);
}


void UInventoryDetailWidget::InitializeDetail_Implementation(FInventoryRecord Record, FName RecordId, UInventoryAsset* Asset)
{
	if (!IsValid(Asset))
	{
		if (IsValid(DetailSwitcher)) DetailSwitcher->SetActiveWidgetIndex(0);
		LOG_ERROR(LogTemp, "Asset is null");
		return;
	}

	GET_GAMEINSTANCESUBSYSTEM_FROM_GAMEINSTANCE(UInventorySubsystem, InventorySubsystem);

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
	if(IsValid(InventoryAsset))
	{
		if (IsValid(AssetTitle)) AssetTitle->SetText(InventoryAsset->AssetName);
		if (IsValid(AssetDescription)) AssetDescription->SetText(InventoryAsset->AssetDescription);
		if (IsValid(AssetImage) && InventoryAsset->AssetIcon.IsValid()) AssetImage->SetBrushFromSoftTexture(InventoryAsset->AssetIcon);

		if (IsValid(AssetTypeWidget))
		{
			AssetTypeWidget->SetVisibility(AssetTypeVisibility.Contains(InventoryAsset->ItemType) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}

		if (IsValid(RecordRank)) RecordRank->SetText(FText::FromString(FString::FromInt(InventoryRecord.EnhanceRecord.Rank)));
		if (IsValid(RecordLevel)) RecordLevel->SetText(FText::FromString(FString::FromInt(InventoryRecord.EnhanceRecord.Level)));
		if (IsValid(RecordExperience)) RecordExperience->SetText(FText::FromString(FString::FromInt(InventoryRecord.EnhanceRecord.Experience)));
	}

	if(IsValid(DetailSwitcher)) DetailSwitcher->SetActiveWidgetIndex(IsValid(InventoryAsset) ? 1 : 0);
}

