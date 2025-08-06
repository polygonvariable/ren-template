// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryCollectionWidget.h"

// Engine Headers
#include "Components/ListView.h"

// Project Headers
#include "RenAsset/Public/Inventory/InventoryAsset.h"

#include "RenCore/Public/Macro/LogMacro.h"

#include "RenInventory/Public/InventorySubsystem.h"
#include "RenInventory/Public/Widget/InventoryEntryObject.h"



void UInventoryCollectionWidget::DisplayItems()
{
	if (!IsValid(EntryObjectClass))
	{
		LOG_ERROR(LogTemp, TEXT("EntryObjectClass is invalid"));
		return;
	}

	if (!IsValid(InventorySubsystem))
	{
		LOG_ERROR(LogTemp, TEXT("InventorySubsystem is invalid"));
		return;
	}

#if WITH_EDITOR
	PROFILE_START(Inventory);
#endif

	int Index = 0;

	InventorySubsystem->QueryItems(FilterRule, QueryRule,
		[this, &Index](const FName& Guid, const FInventoryRecord* Record, UInventoryAsset* Asset)
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
			Entry->ItemGuid = Guid;
			Entry->InventoryAsset = Asset;
			Entry->InventoryRecord = Record;

			Index++;

			// Updating the data doesnt update the UI
			// as the NativeOnListItemObjectSet in UInventoryEntryWidget
			// is not called when the same object is reused.
			// It did worked when creating new objects in each iteration.
			// Also in object pool if .pop() is used to acquire the object, then the order is reversed in each iteration,
			// as the actual widget of UInventoryEntryWidget doesnt update the UInventoryEntryObject ptr in InventoryContainer->AddItem(EntryObject);
			HandleDisplayOfEntry(Entry);
		}
	);

#if WITH_EDITOR
	PROFILE_END(Inventory, 5.0f, TEXT("Inventory rendered in"));
#endif
}

void UInventoryCollectionWidget::ClearItems()
{
	if (InventoryContainer)
	{
		const TArray<UObject*> Items = InventoryContainer->GetListItems();
		for (UObject* Item : Items)
		{
			IObjectPoolInterface* EntryInterface = Cast<IObjectPoolInterface>(Item);
			if (EntryInterface)
			{
				EntryInterface->ReturnToPool();
			}
		}
		InventoryContainer->ClearListItems();
	}
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
		OnItemSelected.Broadcast(Entry->ItemGuid, Entry->InventoryRecord, Entry->InventoryAsset);
	}
}

void UInventoryCollectionWidget::NativeConstruct()
{
	if (InventoryContainer && !InventoryContainer->OnItemSelectionChanged().IsBoundToObject(this))
	{
		InventoryContainer->OnItemSelectionChanged().AddUObject(this, &UInventoryCollectionWidget::HandleSelectedEntry);
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

	/*if (!EntryObjectClass)
	{
		LOG_ERROR(LogTemp, TEXT("EntryObjectClass is invalid"));
		return;
	}

	UPersistentObjectPool* ObjectPool = NewObject<UPersistentObjectPool>(this);
	if (!IsValid(ObjectPool))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to create entry object pool"));
		return;
	}
	EntryObjectPool = ObjectPool;
	EntryObjectPool->SetObjectClass(EntryObjectClass);*/

	Super::NativeConstruct();
}

void UInventoryCollectionWidget::NativeDestruct()
{
	if (IsValid(InventoryContainer))
	{
		InventoryContainer->OnItemSelectionChanged().RemoveAll(this);
	}

	/*if (IsValid(EntryObjectPool))
	{
		EntryObjectPool->Reset<UInventoryEntryObject>();
		EntryObjectPool->MarkAsGarbage();
	}

	EntryObjectPool = nullptr;*/
	InventorySubsystem = nullptr;

	Super::NativeDestruct();
}

