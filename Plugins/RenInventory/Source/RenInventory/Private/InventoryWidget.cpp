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



void UInventoryWidget::DisplayStoredRecords()
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

	if (IsValid(InventoryContainer))
	{
		InventoryContainer->ClearListItems();
	}

	//const TMap<FName, FInventoryRecord>& InventoryItems = InventorySubsystem->BP_GetAllItemRecords();

	/*TArray<FInventorySortEntry> SortedItems;
	for (const auto& Item : InventoryItems)
	{
		const FInventoryRecord& Record = Item.Value;
		UInventoryAsset* Asset = InventorySubsystem->GetItemAsset(Record.ItemId);

		if (Asset && Record.IsValid())
		{
			SortedItems.Emplace(Item.Key, Asset, &Record);
		}
	}*/

	InventorySubsystem->ForEachItem([this](const FName& Guid, const FInventoryRecord& Record, UInventoryAsset* Asset)
		{
			UInventoryEntryObject* Entry = NewObject<UInventoryEntryObject>(this, EntryObjectClass);
			if (!IsValid(Entry))
			{
				LOG_ERROR(LogTemp, TEXT("Failed to create entry object"));
				return;
			}
			Entry->Guid = Guid;
			Entry->Asset = Asset;
			Entry->Record = Record;

			HandleDisplayOfEntry(Entry);
		}
	);

	/*SortedArray.Sort([](const FInventorySortEntry& A, const FInventorySortEntry& B)
		{
			return A.Asset->AssetName.ToString() > B.Asset->AssetName.ToString();
		}
	);
	SortedArray.Sort([](const FInventorySortEntry& A, const FInventorySortEntry& B)
		{
			return A.Asset->ItemRarity > B.Asset->ItemRarity;
		}
	);*/

	/*for (const auto& Item : SortedItems)
	{
		const FName& Guid = Item.Guid;
		UInventoryAsset* Asset = Item.Asset;
		const FInventoryRecord* Record = Item.Record;

		if (Asset && Record)
		{
			UInventoryEntryObject* Entry = NewObject<UInventoryEntryObject>(this, EntryObjectClass);
			if (!IsValid(Entry))
			{
				LOG_ERROR(LogTemp, TEXT("Failed to create entry object"));
				continue;
			}
			Entry->Guid = Guid;
			Entry->Asset = Asset;
			Entry->Record = *Record;

			HandleDisplayOfEntry(Entry);
		}
	}*/

	/*for (const auto& Record : InventoryRecords)
	{
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
	}*/

}

void UInventoryWidget::HandleDisplayOfEntry(UInventoryEntryObject* EntryObject)
{
	if (InventoryContainer)
	{
		InventoryContainer->AddItem(EntryObject);
	}
}

bool UInventoryWidget::HandleEntryFiltering(const FInventoryRecord& InventoryRecord, UInventoryAsset* InventoryAsset)
{
	return false;
}

void UInventoryWidget::HandleSelectedEntry(UObject* Object)
{
	UInventoryEntryObject* Entry = Cast<UInventoryEntryObject>(Object);
	if (!IsValid(Entry))
	{
		return;
	}


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
	if (IsValid(AssetImage)) AssetImage->SetBrushFromSoftTexture(InventoryAsset->ItemIcon);
	if (IsValid(AssetTitle)) AssetTitle->SetText(InventoryAsset->ItemName);
}

void UInventoryEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (!IsValid(ListItemObject))
	{
		return;
	}
	UInventoryEntryObject* EntryObject = Cast<UInventoryEntryObject>(ListItemObject);
	InventoryRecordId = EntryObject->Guid;
	InventoryRecord = EntryObject->Record;
	InventoryAsset = EntryObject->Asset;

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

