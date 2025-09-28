// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryCollectionWidget.h"

// Engine Headers
#include "Components/ListView.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"

#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreFilter/Public/FilterGroup.h"

#include "RenInventory/Public/InventorySubsystem.h"
#include "RenInventory/Public/Widget/InventoryEntryObject.h"



void UInventoryCollectionWidget::DisplayItems()
{
	if (!IsValid(EntryObjectClass))
	{
		LOG_ERROR(LogTemp, TEXT("EntryObjectClass is invalid"));
		return;
	}

	UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (!IsValid(Subsystem))
	{
		LOG_ERROR(LogTemp, TEXT("InventorySubsystem is invalid"));
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

	Subsystem->QueryItems(CriterionRoot, QueryRule,
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

void UInventoryCollectionWidget::ClearItems()
{
	if (InventoryContainer)
	{
		const TArray<UObject*> Items = InventoryContainer->GetListItems();
		for (UObject* Item : Items)
		{
			UInventoryEntryObject* EntryObject = Cast<UInventoryEntryObject>(Item);
			if (EntryObject)
			{
				EntryObject->ResetData();
			}
		}
		InventoryContainer->ClearListItems();
	}
}

void UInventoryCollectionWidget::RefreshItems()
{
	ClearItems();
	DisplayItems();
}

UInventoryEntryObject* UInventoryCollectionWidget::GetSelectedItem()
{
	if (!InventoryContainer)
	{
		return nullptr;
	}
	return InventoryContainer->GetSelectedItem<UInventoryEntryObject>();
}


void UInventoryCollectionWidget::AddPayload(const FPrimaryAssetId& AssetId, FInstancedStruct Payload)
{
	InventoryPayloads.Add(AssetId, Payload);
}

void UInventoryCollectionWidget::SetPayloads(const TMap<FPrimaryAssetId, FInstancedStruct>& Payloads)
{
	InventoryPayloads = Payloads;
}

void UInventoryCollectionWidget::ClearPayloads()
{
	InventoryPayloads.Empty();
}

void UInventoryCollectionWidget::ConstructEntry(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	// man i give up on the implementation of object pooling for now.
	// 
	// so when the .ClearListItems() is called it removes all object entries but they cannot be Garbage Collected.
	// but when .RemoveItem() is called the objects can be Garbage Collected.
	// so i thought of removing items individually using .GetListItems() & .RemoveItem() and it doesnt GC the entry object, WHAT!
	// 
	// another problem is that .AddItem() doesnt seem to update its' entry object ptr if the object is valid & was used before.
	// for example in 2nd iteration of display items, the display order could be wrong as the object reference didnt update,
	// this can also cause list to render is reverse order in each iteration.
	// 
	// UInventoryEntryObject* Entry = EntryObjectPool->AcquireObject<UInventoryEntryObject>(Index, this);
	UInventoryEntryObject* Entry = NewObject<UInventoryEntryObject>(this, EntryObjectClass);
	if (!IsValid(Entry))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to create entry object"));
		return;
	}
	Entry->AssetId = AssetId;
	Entry->Quantity = Quantity;
	Entry->Record = Record;

	/*if (bEnablePayloads)
	{
		Entry->bEnablePayload = true;
		Entry->InventoryPayload = InventoryPayloads.FindRef(AssetId.PrimaryAssetName);
	}*/

	// Updating the data doesnt update the UI
	// as the NativeOnListItemObjectSet in UInventoryEntryWidget
	// is not called when the same object is reused.
	// It did worked when creating new objects in each iteration.
	// Also in object pool if .pop() is used to acquire the object, then the order is reversed in each iteration,
	// as the actual widget of UInventoryEntryWidget doesnt update the UInventoryEntryObject ptr in InventoryContainer->AddItem(EntryObject);
	HandleDisplayOfEntry(Entry);
}

void UInventoryCollectionWidget::HandleDisplayOfEntry(UInventoryEntryObject* EntryObject)
{
	if (InventoryContainer)
	{
		InventoryContainer->AddItem(EntryObject);
	}
}

void UInventoryCollectionWidget::HandleSelectedEntry(UObject* Object)
{
	UInventoryEntryObject* Entry = Cast<UInventoryEntryObject>(Object);
	if (IsValid(Entry))
	{
		OnItemSelected.Broadcast(
			Entry->AssetId,
			Entry->Quantity,
			Entry->Record
		);
	}
}

void UInventoryCollectionWidget::NativeConstruct()
{
	if (InventoryContainer)
	{
		if (!InventoryContainer->OnItemSelectionChanged().IsBoundToObject(this))
		{
			InventoryContainer->OnItemSelectionChanged().AddUObject(this, &UInventoryCollectionWidget::HandleSelectedEntry);
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UInventorySubsystem* Subsystem = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (!IsValid(Subsystem))
		{
			LOG_ERROR(LogTemp, TEXT("InventorySubsystem is invalid"));
			return;
		}

		if (bAutoRefresh)
		{
			Subsystem->OnItemAdded.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId& AssetId, const FInventoryRecord* Record)	{ if (QueryRule.ContainerId == ContainerId && bAutoRefresh) RefreshItems(); });
			Subsystem->OnItemRemoved.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId& AssetId, const FInventoryRecord* Record)	{ if (QueryRule.ContainerId == ContainerId && bAutoRefresh) RefreshItems(); });
			Subsystem->OnItemUpdated.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId& AssetId, const FInventoryRecord* Record)	{ if (QueryRule.ContainerId == ContainerId && bAutoRefresh) RefreshItems(); });
		}

		InventorySubsystem = Subsystem;
	}

	Super::NativeConstruct();
}

void UInventoryCollectionWidget::NativeDestruct()
{
	if (InventoryContainer)
	{
		InventoryContainer->OnItemSelectionChanged().RemoveAll(this);
	}

	UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (IsValid(Subsystem))
	{
		Subsystem->OnItemAdded.RemoveAll(this);
		Subsystem->OnItemRemoved.RemoveAll(this);
		Subsystem->OnItemUpdated.RemoveAll(this);
	}
	InventorySubsystem.Reset();

	Super::NativeDestruct();
}

