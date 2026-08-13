// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/InventoryStorageManager.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Core/Type/AssetFilterProperty.h"
#include "Definition/FilterContext.h"
#include "Core/Type/InventoryFilterProperty.h"
#include "Core/Type/InventoryQueryType.h"
#include "Core/Type/InventorySortType.h"
#include "Filter/FilterCriterion.h"
#include "Core/InventoryPrimaryAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Core/InventorySettings.h"
#include "Data/InventoryStorage.h"
#include "System/AssetInstanceRelationSubsystem.h"


bool UInventoryStorageManager::AddInstance(const FPrimaryAssetId& AssetId, int Quantity)
{
	FInventoryStack* Stack = FindOrAddStack(AssetId);
	return AddItem_Internal(AssetId, Quantity, Stack);
}

bool UInventoryStorageManager::AddInstances(const TMap<FPrimaryAssetId, int>& Items, int Multiplier)
{
	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int Quantity = Kv.Value * Multiplier;

		FInventoryStack* Stack = FindOrAddStack(AssetId);
		AddItem_Internal(AssetId, Quantity, Stack);
	}

	return true;
}


bool UInventoryStorageManager::RemoveInstance(const FPrimaryAssetId& AssetId, int Quantity)
{
	FInventoryStack* Stack = FindOrAddStack(AssetId);
	return RemoveItem_Internal(AssetId, Quantity, Stack);
}

bool UInventoryStorageManager::RemoveInstances(const TMap<FPrimaryAssetId, int>& Items, int Multiplier)
{
	if (!ContainInstances(Items, Multiplier))
	{
		LOG_ERROR(LogInventory, TEXT("Items not contain"));
		return false;
	}

	bool bResult = true;

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int Quantity = Kv.Value * Multiplier;

		FInventoryStack* Stack = FindOrAddStack(AssetId);
		bool bRemoved = RemoveItem_Internal(AssetId, Quantity, Stack);
		if (!bRemoved)
		{
			bResult = false;
			break;
		}
	}

	return true;
}

bool UInventoryStorageManager::RemoveAnyInstances(const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity)
{
	FPrimaryAssetId SelectedAssetId;
	int SelectedQuantity = 0;

	if (!ContainAnyInstances(InItems, InMultiplier, SelectedAssetId, SelectedQuantity))
	{
		LOG_ERROR(LogInventory, TEXT("Items not contain"));
		return false;
	}

	bool bRemoved = RemoveInstance(SelectedAssetId, SelectedQuantity);
	if (!bRemoved)
	{
		LOG_ERROR(LogInventory, TEXT("Items not contain"));
		return false;
	}

	OutAssetId = SelectedAssetId;
	OutQuantity = SelectedQuantity;
	return true;
}

bool UInventoryStorageManager::RemoveInstanceById(const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity)
{
	FInventoryStack* Stack = FindOrAddStack(AssetId);
	return RemoveItemById_Internal(AssetId, ItemId, Quantity, Stack);
}


const FAscensionData* UInventoryStorageManager::GetAscensionInstance(const FPrimaryAssetId& AssetId, const FGuid& InstanceId) const
{
	const FInventoryInstance* Instance = GetInstanceById(AssetId, InstanceId);
	if (!Instance)
	{
		return nullptr;
	}
	return &Instance->Ascension;
}

bool UInventoryStorageManager::UpdateInstance(const FPrimaryAssetId& AssetId, TFunctionRef<void(FInventoryInstance*)> Callback)
{
	FInventoryInstance* Item = GetMutableItemByIndex(AssetId, 0);
	if (!Item)
	{
		return false;
	}

	Callback(Item);
	OnStorageUpdated.Broadcast();
	return true;
}

bool UInventoryStorageManager::UpdateInstanceById(const FPrimaryAssetId& AssetId, const FGuid& ItemId, TFunctionRef<void(FInventoryInstance*)> Callback)
{
	FInventoryInstance* Item = GetMutableItemById(AssetId, ItemId);
	if (!Item)
	{
		return false;
	}

	Callback(Item);
	OnStorageUpdated.Broadcast();
	return true;
}



bool UInventoryStorageManager::ContainInstance(const FPrimaryAssetId& AssetId, int Quantity) const
{
	const FInventoryStack* Stack = GetStack(AssetId);
	if (!Stack)
	{
		return false;
	}
	return Stack->ItemList.Num() >= Quantity;
}

bool UInventoryStorageManager::ContainInstances(const TMap<FPrimaryAssetId, int>& Items, int Multiplier) const
{
	bool bResult = true;

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int RequiredQuantity = Kv.Value * Multiplier;

		if (!FInventoryPrimaryAsset::IsValid(AssetId) || RequiredQuantity <= 0)
		{
			bResult = false;
			break;
		}

		const FInventoryStack* Stack = GetStack(AssetId);
		if (!Stack)
		{
			bResult = false;
			break;
		}

		const TArray<FInventoryInstance>& ItemList = Stack->ItemList;
		bool bStackable = Stack->bStackable;
		int ItemQuantity = 0;
		if (bStackable)
		{
			if (!ItemList.IsValidIndex(0))
			{
				bResult = false;
				break;
			}

			ItemQuantity = ItemList[0].Quantity;
		}
		else
		{
			ItemQuantity = ItemList.Num();
		}

		if (ItemQuantity < RequiredQuantity)
		{
			bResult = false;
			break;
		}
	}

	return bResult;
}

bool UInventoryStorageManager::ContainAnyInstances(const TMap<FPrimaryAssetId, int>& InItems, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) const
{
	bool bResult = false;

	for (const TPair<FPrimaryAssetId, int>& Kv : InItems)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int RequiredQuantity = Kv.Value * InMultiplier;

		if (!FInventoryPrimaryAsset::IsValid(AssetId) || RequiredQuantity <= 0)
		{
			continue;
		}

		const FInventoryStack* Stack = GetStack(AssetId);
		if (!Stack)
		{
			continue;
		}

		const TArray<FInventoryInstance>& ItemList = Stack->ItemList;
		bool bStackable = Stack->bStackable;
		int ItemQuantity = 0;
		if (bStackable)
		{
			if (!ItemList.IsValidIndex(0))
			{
				continue;
			}

			const FInventoryInstance& Record = ItemList[0];
			ItemQuantity = Record.Quantity;
		}
		else
		{
			ItemQuantity = ItemList.Num();
		}

		if (ItemQuantity >= RequiredQuantity)
		{
			OutAssetId = AssetId;
			OutQuantity = RequiredQuantity;

			bResult = true;
			break;
		}
	}

	return bResult;
}

FGameEventDelegate& UInventoryStorageManager::GetOnAssetInstanceCollectionUpdated()
{
	return OnStorageUpdated;
}



int UInventoryStorageManager::GetTotalQuantity(const FPrimaryAssetId& AssetId) const
{
	return GetItemQuantity_Internal(AssetId);
}

const FInventoryInstance* UInventoryStorageManager::GetInstance(const FPrimaryAssetId& AssetId) const
{
	const FInventoryStack* Stack = GetStack(AssetId);
	if (!Stack)
	{
		return nullptr;
	}

	const TArray<FInventoryInstance>& RecordList = Stack->ItemList;
	if (RecordList.Num() <= 0)
	{
		return nullptr;
	}

	return &RecordList[0];
}

const FInventoryInstance* UInventoryStorageManager::GetInstanceById(const FPrimaryAssetId& AssetId, const FGuid& ItemId) const
{
	const FInventoryStack* Stack = GetStack(AssetId);
	if (!Stack)
	{
		return nullptr;
	}

	const TArray<FInventoryInstance>& RecordList = Stack->ItemList;
	const FInventoryInstance* Record = RecordList.FindByPredicate([ItemId](const FInventoryInstance& Record) { return Record.ItemId == ItemId; });

	return Record;
}



void UInventoryStorageManager::QueryInstances(const UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TArray<FInventorySortEntry>& OutSortedItems)
{
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!IsValid(AssetManager))
	{
		return;
	}

	if (QueryRule.QuerySource == EAssetQuerySource::Asset)
	{
		QueryAssetItems(AssetManager, FilterCriterion, OutSortedItems);
	}
	else
	{
		QueryInstanceItems(AssetManager, FilterCriterion, OutSortedItems);
	}

	HandleItemSorting(OutSortedItems, QueryRule);
}


FInventoryInstance* UInventoryStorageManager::GetMutableItemByIndex(const FPrimaryAssetId& AssetId, int Index)
{
	FInventoryStack* Stack = FindOrAddStack(AssetId);
	if (!Stack)
	{
		return nullptr;
	}

	TArray<FInventoryInstance>& ItemList = Stack->ItemList;
	if (!ItemList.IsValidIndex(Index))
	{
		return nullptr;
	}

	return &ItemList[Index];
}

FInventoryInstance* UInventoryStorageManager::GetMutableItemById(const FPrimaryAssetId& AssetId, const FGuid& ItemId)
{
	FInventoryStack* Stack = FindOrAddStack(AssetId);
	if (!Stack)
	{
		return nullptr;
	}

	TArray<FInventoryInstance>& ItemList = Stack->ItemList;
	FInventoryInstance* Item = ItemList.FindByPredicate([ItemId](const FInventoryInstance& Record) { return Record.ItemId == ItemId; });

	return Item;
}


void UInventoryStorageManager::HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const
{
	ESortDirection SortDirection = QueryRule.SortDirection;
	EInventorySortType SortType = QueryRule.SortType;

	switch (SortType)
	{
	case EInventorySortType::None:
		LOG_WARNING(LogInventory, TEXT("No sort implemented"));
		break;
	case EInventorySortType::Alphabetical:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
				return (SortDirection == ESortDirection::Ascending) ?
					A.DisplayName.ToString() < B.DisplayName.ToString() :
					A.DisplayName.ToString() > B.DisplayName.ToString();
			}
		);
		break;
	case EInventorySortType::Quantity:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
				return (SortDirection == ESortDirection::Ascending) ?
					A.Quantity < B.Quantity :
					A.Quantity > B.Quantity;
			}
		);
		break;
	case EInventorySortType::Level:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
				if (!A.Instance || !B.Instance) return false;
				return (SortDirection == ESortDirection::Ascending) ?
					A.Instance->Ascension.Level < B.Instance->Ascension.Level :
					A.Instance->Ascension.Level > B.Instance->Ascension.Level;
			}
		);
		break;
	case EInventorySortType::Rank:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
				if (!A.Instance || !B.Instance) return false;
				return (SortDirection == ESortDirection::Ascending) ?
					A.Instance->Ascension.Rank < B.Instance->Ascension.Rank :
					A.Instance->Ascension.Rank > B.Instance->Ascension.Rank;
			}
		);
		break;
	default:
		LOG_WARNING(LogInventory, TEXT("No sort implemented"));
		break;
	}
}

void UInventoryStorageManager::QueryAssetItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FInventorySortEntry>& OutSortedItems) const
{
	TArray<FPrimaryAssetId> AssetIds;
	if (!AssetManager->GetPrimaryAssetIdList(FInventoryPrimaryAsset::GetAssetType(), AssetIds))
	{
		return;
	}

	OutSortedItems.Reserve(AssetIds.Num());

	for (const FPrimaryAssetId& AssetId : AssetIds)
	{
		FAssetData AssetData;
		if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
		{
			continue;
		}

		FText DisplayName = FText::GetEmpty();
		FName ItemType = NAME_None;
		FName ItemRarity = NAME_None;

		FInventoryPrimaryAsset::GetDisplayName(AssetData, DisplayName);
		FInventoryPrimaryAsset::GetType(AssetData, ItemType);
		FInventoryPrimaryAsset::GetRarity(AssetData, ItemRarity);

		if (IsValid(FilterCriterion))
		{
			FFilterContext Context;
			Context.SetValue(FAssetFilterProperty::AssetId, AssetId);
			Context.SetValue(FInventoryFilterProperty::ItemType, ItemType);
			Context.SetValue(FInventoryFilterProperty::ItemRarity, ItemRarity);

			if (!FilterCriterion->Evaluate(Context))
			{
				continue;
			}
		}

		int ItemQuantity = GetItemQuantity_Internal(AssetId);

		OutSortedItems.Emplace(AssetId, DisplayName, ItemQuantity);
	}
}

void UInventoryStorageManager::QueryInstanceItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FInventorySortEntry>& OutSortedItems) const
{
	UAssetInstanceRelationSubsystem* RelationSubsystem = UAssetInstanceRelationSubsystem::Get(GetWorld());
	if (!IsValid(LocalStorage) || !IsValid(RelationSubsystem))
	{
		LOG_ERROR(LogInventory, TEXT("Inventory Storage or Asset Instance Relation is invalid"));
		return;
	}

	const TMap<FPrimaryAssetId, FInventoryStack>& InventoryStack = LocalStorage->InventoryStack;

	for (const TPair<FPrimaryAssetId, FInventoryStack>& Kv : InventoryStack)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		const FInventoryStack* Stack = &Kv.Value;
		if (!FInventoryPrimaryAsset::IsValid(AssetId) || !Stack)
		{
			continue;
		}

		FAssetData AssetData;
		if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
		{
			continue;
		}

		FText DisplayName = FText::GetEmpty();
		FName ItemType = TEXT_EMPTY;
		FName ItemRarity = TEXT_EMPTY;

		bool bNameValid = FInventoryPrimaryAsset::GetDisplayName(AssetData, DisplayName);
		bool bTypeValid = FInventoryPrimaryAsset::GetType(AssetData, ItemType);
		bool bRarityValid = FInventoryPrimaryAsset::GetRarity(AssetData, ItemRarity);
		if (!bNameValid || !bTypeValid || !bRarityValid)
		{
			continue;
		}

		const TArray<FInventoryInstance>& ItemList = Stack->ItemList;
		for (const FInventoryInstance& Item : ItemList)
		{
			int ItemQuantity = Item.Quantity;
			if (IsValid(FilterCriterion))
			{
				FFilterContext Context;
				Context.SetValue(FAssetFilterProperty::AssetId, AssetId);
				Context.SetValue(FInventoryFilterProperty::ItemType, ItemType);
				Context.SetValue(FInventoryFilterProperty::ItemRarity, ItemRarity);
				Context.SetValue(FInventoryFilterProperty::ItemId, FName(*Item.ItemId.ToString()));
				Context.SetValue(FInventoryFilterProperty::ItemQuantity, ItemQuantity);

				if (!FilterCriterion->Evaluate(Context))
				{
					continue;
				}
			}

			bool bIsLinked = RelationSubsystem->HasAnyLinkedRelation(AssetId, Item.ItemId);
			OutSortedItems.Emplace(AssetId, DisplayName, ItemQuantity, &Item, bIsLinked);
		}
	}
}


const FInventoryStack* UInventoryStorageManager::GetStack(const FPrimaryAssetId& AssetId) const
{
	if (!IsValid(LocalStorage))
	{
		return nullptr;
	}
	return LocalStorage->InventoryStack.Find(AssetId);
}

FInventoryStack* UInventoryStorageManager::FindOrAddStack(const FPrimaryAssetId& AssetId)
{
	if (!IsValid(LocalStorage))
	{
		LOG_ERROR(LogInventory, TEXT("Inventory Storage is invalid"));
		return nullptr;
	}

	TMap<FPrimaryAssetId, FInventoryStack>& InventoryStack = LocalStorage->InventoryStack;
	FInventoryStack* Stack = InventoryStack.Find(AssetId);
	if (Stack)
	{
		return Stack;
	}

	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!IsValid(AssetManager))
	{
		return nullptr;
	}

	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
	{
		return nullptr;
	}

	bool bStackable = false;
	bool bPersistWhenEmpty = false;

	bool bFoundStackable = FInventoryPrimaryAsset::GetStackable(AssetData, bStackable);
	bool bFoundPersist = FInventoryPrimaryAsset::GetPersistWhenEmpty(AssetData, bPersistWhenEmpty);
	if (!bFoundStackable || !bFoundPersist)
	{
		return nullptr;
	}

	FGuid StackId = FGuid::NewGuid();
	FInventoryStack& NewStack = InventoryStack.Add(AssetId, {StackId, bStackable, bPersistWhenEmpty});

	return &NewStack;
}



int UInventoryStorageManager::GetItemQuantity_Internal(const FPrimaryAssetId& AssetId) const
{
	const FInventoryStack* Stack = GetStack(AssetId);
	if (!Stack)
	{
		return 0;
	}

	const TArray<FInventoryInstance>& ItemList = Stack->ItemList;
	bool bStackable = Stack->bStackable;
	if (bStackable)
	{
		if (!ItemList.IsValidIndex(0))
		{
			return 0;
		}
		return ItemList[0].Quantity;
	}

	return ItemList.Num();
}



bool UInventoryStorageManager::AddItem_Internal(const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack)
{
	if (!FInventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0 || !Stack)
	{
		LOG_ERROR(LogInventory, TEXT("AssetId, Stack is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TArray<FInventoryInstance>& ItemList = Stack->ItemList;
	bool bStackable = Stack->bStackable;
	if (bStackable)
	{
		if (ItemList.IsValidIndex(0))
		{
			FInventoryInstance& ItemData = ItemList[0];
			AddItem_InternalUpdate(AssetId, ItemData, Quantity);
			return true;
		}
		else
		{
			AddItem_InternalCreate(AssetId, ItemList, Quantity);
			return true;
		}
	}

	for (int i = 0; i < Quantity; i++)
	{
		AddItem_InternalCreate(AssetId, ItemList, 1);
	}

	return true;
}

void UInventoryStorageManager::AddItem_InternalCreate(const FPrimaryAssetId& AssetId, TArray<FInventoryInstance>& ItemList, int Quantity)
{
	FInventoryInstance Item;
	Item.ItemId = FGuid::NewGuid();
	Item.Quantity = Quantity;
	ItemList.Add(Item);

	OnStorageUpdated.Broadcast();

	LOG_INFO(LogInventory, TEXT("New item added"));
}

void UInventoryStorageManager::AddItem_InternalUpdate(const FPrimaryAssetId& AssetId, FInventoryInstance& Item, int Quantity)
{
	Item.Quantity += Quantity;
	OnStorageUpdated.Broadcast();

	LOG_INFO(LogInventory, TEXT("Item updated"));
}



bool UInventoryStorageManager::RemoveItem_Internal(const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack)
{
	if (!FInventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0 || !Stack)
	{
		LOG_ERROR(LogInventory, TEXT("AssetId, Stack is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TArray<FInventoryInstance>& ItemList = Stack->ItemList;
	bool bStackable = Stack->bStackable;
	if (bStackable)
	{
		if (ItemList.IsValidIndex(0))
		{
			bool bPersist = Stack->bPersistWhenEmpty;

			FInventoryInstance& Record = ItemList[0];
			RemoveItem_InternalUpdate(AssetId, ItemList, bPersist, &Record, Quantity);

			return true;
		}

		LOG_ERROR(LogInventory, TEXT("Not item to remove"));
		return false;
	}

	bool bRemoved = RemoveItem_InternalStack(AssetId, ItemList, Quantity);
	return bRemoved;
}

bool UInventoryStorageManager::RemoveItemById_Internal(const FPrimaryAssetId& AssetId, const FGuid& ItemId, int Quantity, FInventoryStack* Stack)
{
	if (!FInventoryPrimaryAsset::IsValid(AssetId) || !ItemId.IsValid() || Quantity <= 0 || !Stack)
	{
		LOG_ERROR(LogInventory, TEXT("AssetId, ItemId, Stack is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TArray<FInventoryInstance>& ItemList = Stack->ItemList;
	FInventoryInstance* Item = ItemList.FindByPredicate([ItemId](const FInventoryInstance& Item) { return Item.ItemId == ItemId; });
	if (!Item)
	{
		LOG_ERROR(LogInventory, TEXT("Item not found for ItemId"));
		return false;
	}

	bool bStackable = Stack->bStackable;
	if (bStackable)
	{
		bool bPersistWhenEmpty = Stack->bPersistWhenEmpty;
		RemoveItem_InternalUpdate(AssetId, ItemList, bPersistWhenEmpty, Item, Quantity);
		return true;
	}

	bool bRemoved = RemoveItem_InternalRemove(AssetId, ItemList, Item);
	return bRemoved;
}

bool UInventoryStorageManager::RemoveItem_InternalStack(const FPrimaryAssetId& AssetId, TArray<FInventoryInstance>& ItemList, int Quantity)
{
	int StackCount = ItemList.Num();
	if (Quantity > StackCount)
	{
		return false;
	}

	for (int i = 0; i < Quantity; i++)
	{
		FInventoryInstance Item = ItemList.Pop();
		OnStorageUpdated.Broadcast();
	}

	return true;
}

bool UInventoryStorageManager::RemoveItem_InternalRemove(const FPrimaryAssetId& AssetId, TArray<FInventoryInstance>& ItemList, FInventoryInstance* Item)
{
	FInventoryInstance TempItem = *Item;

	int Count = ItemList.Remove(TempItem);
	if (Count == 0)
	{
		return false;
	}

	OnStorageUpdated.Broadcast();
	return true;
}

void UInventoryStorageManager::RemoveItem_InternalUpdate(const FPrimaryAssetId& AssetId, TArray<FInventoryInstance>& ItemList, bool bPersistWhenEmpty, FInventoryInstance* Item, int Quantity)
{
	FGuid ItemId = Item->ItemId;
	int ItemQuantity = Item->Quantity;

	int NewQuantity = ItemQuantity - Quantity;
	if (NewQuantity > 0)
	{
		Item->Quantity = NewQuantity;
		OnStorageUpdated.Broadcast();
		return;
	}

	if (bPersistWhenEmpty)
	{
		Item->Quantity = 0;
		OnStorageUpdated.Broadcast();
		return;
	}

	RemoveItem_InternalRemove(AssetId, ItemList, Item);
}


UObject* UInventoryStorageManager::GetStorage() const
{
	return LocalStorage;
}

void UInventoryStorageManager::SetStorage(UObject* InStorage)
{
	LocalStorage = Cast<UInventoryStorage>(InStorage);
}

void UInventoryStorageManager::OnStorageLoaded(bool bIsNew)
{
	if (bIsNew && IsValid(LocalStorage))
	{
		const UInventorySettings* Settings = UInventorySettings::Get();

		LocalStorage->InventoryStack = Settings->DefaultInventory;
	}
}

FGameEventDelegate& UInventoryStorageManager::GetOnStorageUpdated()
{
	return OnStorageUpdated;
}

