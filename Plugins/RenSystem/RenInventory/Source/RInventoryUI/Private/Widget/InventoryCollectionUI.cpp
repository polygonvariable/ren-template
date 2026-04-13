// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryCollectionUI.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Storage/InventoryStorage.h"
#include "Subsystem/InventorySubsystem.h"
#include "Widget/InventoryEntry.h"


void UInventoryCollectionUI::InitializeCollection()
{
	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(GetGameInstance());
	if (!IsValid(InventorySubsystem))
	{
		return;
	}

	InventoryStorage = InventorySubsystem->GetInventory(PrimarySourceId);
	if (IsValid(InventoryStorage) && bAutoRefresh)
	{
		InventoryStorage->OnStorageUpdated.AddUObject(this, &UInventoryCollectionUI::RefreshEntries);
	}
}

void UInventoryCollectionUI::DisplayEntries()
{
	if (!IsValid(InventoryStorage))
	{
		LOG_ERROR(LogInventory, TEXT("InventoryStorage is invalid"));
		return;
	}

	InventoryStorage->QueryInstances(GetFilterRoot(), QueryRule,
		[this](const FInventorySortEntry& SortEntry)
		{
			UInventoryEntry* Entry = GetEntryFromPool<UInventoryEntry>();
			if (IsValid(Entry))
			{
				Entry->Item = SortEntry.Item;
				Entry->Quantity = SortEntry.Quantity;
				AddEntry(SortEntry.AssetId, Entry);
			}
		}
	);
}

void UInventoryCollectionUI::NativeDestruct()
{
	if (IsValid(InventoryStorage))
	{
		InventoryStorage->OnStorageUpdated.RemoveAll(this);
	}
	InventoryStorage = nullptr;

	Super::NativeDestruct();
}

