// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryCollectionUI.h"

// Engine Headers
#include "Components/ListView.h"

// Project Headers
#include "RCoreFilter/Public/FilterGroup.h"
#include "RCoreFilter/Public/FilterLeafCriterion.h"

#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreFilter/Public/FilterGroup.h"

#include "RenInventory/Public/InventorySubsystem.h"
#include "RenInventory/Public/InventoryEntry.h"



void UInventoryCollectionUI::DisplayItems()
{
	if (!IsValid(EntryObjectClass))
	{
		LOG_ERROR(LogInventory, TEXT("EntryObjectClass is invalid"));
		return;
	}

	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory))
	{
		LOG_ERROR(LogInventory, TEXT("InventorySubsystem is invalid"));
		return;
	}

#if WITH_EDITOR
	TIMER_START(Inventory);
#endif
	
	UFilterCriterion* CriterionRoot = nullptr;
	if (IsValid(FilterRule))
	{
		CriterionRoot = FilterRule->CriterionRoot;
	}

	Inventory->QueryItems(CriterionRoot, QueryRule,
		[this](const FInventorySortEntry& SortEntry)
		{
			ConstructEntry(
				SortEntry.AssetId,
				SortEntry.ItemQuantity,
				SortEntry.Record
			);
		}
	);

#if WITH_EDITOR
	TIMER_END(Inventory, 5.0f, TEXT("Inventory rendered in"));
#endif

}

void UInventoryCollectionUI::ClearItems()
{
	if (InventoryContainer)
	{
		const TArray<UObject*> Items = InventoryContainer->GetListItems();
		for (UObject* Item : Items)
		{
			UInventoryEntry* EntryObject = Cast<UInventoryEntry>(Item);
			if (EntryObject)
			{
				EntryObject->ResetData();
			}
		}
		InventoryContainer->ClearListItems();
	}
}

void UInventoryCollectionUI::RefreshItems()
{
	ClearItems();
	DisplayItems();
}

UInventoryEntry* UInventoryCollectionUI::GetSelectedItem()
{
	if (!InventoryContainer)
	{
		return nullptr;
	}
	return InventoryContainer->GetSelectedItem<UInventoryEntry>();
}


void UInventoryCollectionUI::AddPayload(const FPrimaryAssetId& AssetId, FInstancedStruct Payload)
{
	InventoryPayloads.Add(AssetId, Payload);
}

void UInventoryCollectionUI::SetPayloads(const TMap<FPrimaryAssetId, FInstancedStruct>& Payloads)
{
	InventoryPayloads = Payloads;
}

void UInventoryCollectionUI::ClearPayloads()
{
	InventoryPayloads.Empty();
}

UFilterCriterion* UInventoryCollectionUI::GetCriterionByName(FName Name)
{
	if (!IsValid(FilterRule))
	{
		return nullptr;
	}
	return FilterRule->GetCriterionByName(Name);
}

void UInventoryCollectionUI::ConstructEntry(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	// man i give up on the implementation of object pooling for now.
	// 
	// so when the .ClearListItems() is called it removes all object entries but they cannot be Garbage Collected.
	// but when .RemoveItem() is called the objects can be Garbage Collected.
	// so i thought of removing items individually using .GetListItems() & .RemoveItem() and it doesn't GC the entry object, WHAT!
	// 
	// another problem is that .AddItem() doesn't seem to update its' entry object ptr if the object is valid & was used before.
	// for example in 2nd iteration of display items, the display order could be wrong as the object reference didn't update,
	// this can also cause list to render is reverse order in each iteration.
	// 
	// UInventoryEntry* Entry = EntryObjectPool->AcquireObject<UInventoryEntry>(Index, this);
	UInventoryEntry* Entry = NewObject<UInventoryEntry>(this, EntryObjectClass);
	if (!IsValid(Entry))
	{
		LOG_ERROR(LogInventory, TEXT("Failed to create entry object"));
		return;
	}
	Entry->AssetId = AssetId;
	Entry->Quantity = Quantity;
	Entry->Record = Record;

	if (bEnablePayloads)
	{
		Entry->bHasPayload = true;
		Entry->Payload = InventoryPayloads.FindRef(AssetId);
	}

	// Updating the data doesn't update the UI
	// as the NativeOnListItemObjectSet in UInventoryEntryWidget
	// is not called when the same object is reused.
	// It did worked when creating new objects in each iteration.
	// Also in object pool if .pop() is used to acquire the object, then the order is reversed in each iteration,
	// as the actual widget of UInventoryEntryWidget doesn't update the UInventoryEntry ptr in InventoryContainer->AddItem(EntryObject);
	HandleDisplayOfEntry(Entry);
}

void UInventoryCollectionUI::HandleDisplayOfEntry(UInventoryEntry* EntryObject)
{
	if (InventoryContainer)
	{
		InventoryContainer->AddItem(EntryObject);
	}
}

void UInventoryCollectionUI::HandleSelectedEntry(UObject* Object)
{
	UInventoryEntry* Entry = Cast<UInventoryEntry>(Object);
	if (IsValid(Entry))
	{
		OnItemSelected.Broadcast(
			Entry->AssetId,
			Entry->Quantity,
			Entry->Record
		);
	}
}

void UInventoryCollectionUI::NativeConstruct()
{
	if (InventoryContainer)
	{
		if (!InventoryContainer->OnItemSelectionChanged().IsBoundToObject(this))
		{
			InventoryContainer->OnItemSelectionChanged().AddUObject(this, &UInventoryCollectionUI::HandleSelectedEntry);
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UInventorySubsystem* Inventory = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (!IsValid(Inventory))
		{
			LOG_ERROR(LogInventory, TEXT("InventorySubsystem is invalid"));
			return;
		}

		if (bAutoRefresh)
		{
			Inventory->OnItemAdded.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId&, FName)		{ if (QueryRule.ContainerId == ContainerId && bAutoRefresh) RefreshItems(); });
			Inventory->OnItemRemoved.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId&, FName)	{ if (QueryRule.ContainerId == ContainerId && bAutoRefresh) RefreshItems(); });
			Inventory->OnItemUpdated.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId&, FName)	{ if (QueryRule.ContainerId == ContainerId && bAutoRefresh) RefreshItems(); });
		}

		InventorySubsystem = Inventory;
	}

	Super::NativeConstruct();
}

void UInventoryCollectionUI::NativeDestruct()
{
	if (InventoryContainer)
	{
		InventoryContainer->OnItemSelectionChanged().RemoveAll(this);
	}

	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (IsValid(Inventory))
	{
		Inventory->OnItemAdded.RemoveAll(this);
		Inventory->OnItemRemoved.RemoveAll(this);
		Inventory->OnItemUpdated.RemoveAll(this);
	}
	InventorySubsystem.Reset();

	Super::NativeDestruct();
}

