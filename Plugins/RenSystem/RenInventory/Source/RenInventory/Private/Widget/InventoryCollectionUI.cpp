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
#include "RCorePool/Public/PoolSubsystem.h"

#include "RenInventory/Public/InventoryEntry.h"
#include "RenInventory/Public/InventorySubsystem.h"



void UInventoryCollectionUI::DisplayItems()
{
	UPoolSubsystem* Pool = PoolSubsystem.Get();
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Pool) || !IsValid(Inventory) || !IsValid(EntryClass) || !ItemList)
	{
		LOG_ERROR(LogInventory, TEXT("PoolSubsystem, InventorySubsystem, EntryClass, ItemList is invalid"));
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
		[this, Pool](const FInventorySortEntry& SortEntry)
		{
			UInventoryEntry* Entry = Pool->AcquireObject<UInventoryEntry>();
			//UInventoryEntry* Entry = NewObject<UInventoryEntry>(this, EntryClass);
			if (Entry)
			{
				Entry->AssetId = SortEntry.AssetId;
				Entry->Quantity = SortEntry.ItemQuantity;
				Entry->Record = SortEntry.Record;

				if (bEnablePayloads)
				{
					Entry->bHasPayload = true;
					Entry->Payload = Payloads.FindRef(SortEntry.AssetId);
				}

				ItemList->AddItem(Entry);
			}
		}
	);

#if WITH_EDITOR
	TIMER_END(Inventory, 5.0f, TEXT("Inventory rendered in"));
#endif

}

void UInventoryCollectionUI::ClearItems()
{
	UPoolSubsystem* Pool = PoolSubsystem.Get();
	if (!IsValid(Pool) || !ItemList)
	{
		return;
	}

	const TArray<UObject*>& Items = ItemList->GetListItems();
	for (UObject* Item : Items)
	{
		UInventoryEntry* Entry = Cast<UInventoryEntry>(Item);
		if (!IsValid(Entry))
		{
			continue;
		}
		Entry->ResetData();
		Pool->ReturnToPool<UInventoryEntry>(Entry);
	}

	ItemList->ClearListItems();
	ItemList->RegenerateAllEntries();
}

void UInventoryCollectionUI::RefreshItems()
{
	ClearItems();
	DisplayItems();
}

UInventoryEntry* UInventoryCollectionUI::GetSelectedItem()
{
	if (!ItemList)
	{
		return nullptr;
	}
	return ItemList->GetSelectedItem<UInventoryEntry>();
}


void UInventoryCollectionUI::AddPayload(const FPrimaryAssetId& AssetId, FInstancedStruct Payload)
{
	Payloads.Add(AssetId, Payload);
}

void UInventoryCollectionUI::ClearPayloads()
{
	Payloads.Empty();
}

UFilterCriterion* UInventoryCollectionUI::GetCriterionByName(FName Name)
{
	if (!IsValid(FilterRule))
	{
		return nullptr;
	}
	return FilterRule->GetCriterionByName(Name);
}


void UInventoryCollectionUI::HandleSelectedItem(UObject* Object)
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
	if (ItemList)
	{
		ItemList->OnItemSelectionChanged().RemoveAll(this);
		ItemList->OnItemSelectionChanged().AddUObject(this, &UInventoryCollectionUI::HandleSelectedItem);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		PoolSubsystem = GameInstance->GetSubsystem<UPoolSubsystem>();
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
	ClearItems();

	if (ItemList)
	{
		ItemList->OnItemSelectionChanged().RemoveAll(this);
	}

	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (IsValid(Inventory))
	{
		Inventory->OnItemAdded.RemoveAll(this);
		Inventory->OnItemRemoved.RemoveAll(this);
		Inventory->OnItemUpdated.RemoveAll(this);
	}
	InventorySubsystem.Reset();
	PoolSubsystem.Reset();

	Super::NativeDestruct();
}

