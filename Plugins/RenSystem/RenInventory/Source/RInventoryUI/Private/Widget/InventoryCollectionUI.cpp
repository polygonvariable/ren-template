// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryCollectionUI.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/InventoryStorageManager.h"
#include "System/InventorySubsystem.h"
#include "Widget/InventoryEntry.h"


void UInventoryCollectionUI::InitializeCollection()
{
	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(GetGameInstance());
	if (!IsValid(InventorySubsystem))
	{
		return;
	}

	StorageManager = InventorySubsystem->GetStorageManager(PrimarySourceId);
	if (IsValid(StorageManager) && bAutoRefresh)
	{
		StorageManager->OnStorageUpdated.AddUObject(this, &UInventoryCollectionUI::RefreshEntries);
	}
}

void UInventoryCollectionUI::DisplayEntries()
{
	if (!IsValid(StorageManager))
	{
		LOG_ERROR(LogInventory, TEXT("InventoryStorage is invalid"));
		return;
	}

	TArray<FInventorySortEntry> SortedItems;
	StorageManager->QueryInstances(GetFilterRoot(), QueryRule, SortedItems);
	for (const FInventorySortEntry& SortItem : SortedItems)
	{
		UInventoryEntry* Entry = GetEntryFromPool<UInventoryEntry>();
		if (IsValid(Entry))
		{
			Entry->Item = SortItem.Instance;
			Entry->Quantity = SortItem.Quantity;
			Entry->bIsLinked = SortItem.bIsLinked;
			AddEntry(SortItem.AssetId, Entry);
		}
	}
}

void UInventoryCollectionUI::NativeDestruct()
{
	if (IsValid(StorageManager))
	{
		StorageManager->OnStorageUpdated.RemoveAll(this);
	}
	StorageManager = nullptr;

	Super::NativeDestruct();
}

