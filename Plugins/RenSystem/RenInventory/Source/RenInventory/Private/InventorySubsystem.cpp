// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InventorySubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "RCoreDelegate/Public/LatentDelegates.h"

#include "RCoreFilter/Public/FilterContext.h"
#include "RCoreFilter/Public/FilterCriterion.h"

#include "RCoreInventory/Public/InventoryContainer.h"
#include "RCoreInventory/Public/InventoryAsset.h"
#include "RCoreInventory/Public/InventoryItemType.h"
#include "RCoreInventory/Public/InventoryProviderInterface.h"
#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RCoreStorage/Public/StorageUtils.h"

#include "RenInventory/Public/InventoryDefinition.h"
#include "RenInventory/Public/InventoryPrimaryAsset.h"



bool UInventorySubsystem::AddItem(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("ContainerId, AssetId is invalid or Quantity is less than or equal to 0"));
		return false;
	}

	FInventoryStack* Stack = GetMutableInventoryStack(ContainerId, AssetId);
	if (!Stack)
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("Stack is invalid"));
		return false;
	}

	bool bAdded = AddItemRecord(ContainerId, AssetId, Quantity, Stack);
	if (!bAdded)
	{
		return false;
	}

	return true;
}

bool UInventorySubsystem::AddItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items)
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!ContainerId.IsValid() || !InventoryInterface)
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("ContainerId or InventoryInterface is invalid"));
		return false;
	}

	FName CurrentItemId = TEXT_EMPTY;
	FInventoryStack* CurrentStack = nullptr;

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId AssetId = Kv.Key;
		int Quantity = Kv.Value;

		if (!InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
		{
			continue;
		}

		if (CurrentItemId != AssetId.PrimaryAssetName)
		{
			CurrentItemId = AssetId.PrimaryAssetName;
			CurrentStack = GetMutableInventoryStack(ContainerId, AssetId);
		}

		bool bAdded = AddItemRecord(ContainerId, AssetId, Quantity, CurrentStack);
	}

	return true;
}


bool UInventorySubsystem::RemoveItem(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("ContainerId, AssetId is invalid or Quantity is less than or equal to 0"));
		return false;
	}

	FInventoryStack* Stack = GetMutableInventoryStack(ContainerId, AssetId);
	if (!Stack)
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("Stack is invalid"));
		return false;
	}

	bool bRemoved = RemoveItemRecord(ContainerId, AssetId, Quantity, Stack);
	return bRemoved;
}

bool UInventorySubsystem::RemoveItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items)
{
	return RemoveItems(ContainerId, Items, 1);
}

bool UInventorySubsystem::RemoveItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier)
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!ContainerId.IsValid() || !InventoryInterface)
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("ContainerId, InventoryInterface is invalid"));
		return false;
	}

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId AssetId = Kv.Key;
		int Quantity = Kv.Value * Multiplier;

		if (!InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
		{
			continue;
		}

		FName CurrentItemId = AssetId.PrimaryAssetName;
		FInventoryStack* CurrentStack = GetMutableInventoryStack(ContainerId, AssetId, InventoryInterface);

		bool bRemoved = RemoveItemRecord(ContainerId, AssetId, Quantity, CurrentStack);
		if (!bRemoved)
		{
			return false;
		}
	}

	return true;
}

bool UInventorySubsystem::RemoveItemById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, int Quantity)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("ContainerId, AssetId is invalid or Quantity is less than or equal to 0"));
		return false;
	}

	FInventoryStack* Stack = GetMutableInventoryStack(ContainerId, AssetId);
	if (!Stack)
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("Stack is invalid"));
		return false;
	}

	bool bRemoved = RemoveItemRecordById(ContainerId, AssetId, RecordId, Quantity, Stack);
	return bRemoved;
}


bool UInventorySubsystem::ContainsItem(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity) const
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
	{
		return false;
	}

	const FInventoryStack* Stack = GetInventoryStack(ContainerId, AssetId);
	const TArray<FInventoryRecord>* RecordList = GetRecords(ContainerId, Stack);
	if (!Stack && !RecordList)
	{
		return false;
	}

	bool bStackable = Stack->bStackable;
	if (bStackable)
	{
		if (!RecordList->IsValidIndex(0))
		{
			return false;
		}

		const FInventoryRecord& Record = (*RecordList)[0];
		return Record.Quantity >= Quantity;
	}

	return RecordList->Num() >= Quantity;
}

bool UInventorySubsystem::ContainsItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items) const
{
	return ContainsItems(ContainerId, Items, 1);
}

bool UInventorySubsystem::ContainsItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items, int Multiplier) const
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!ContainerId.IsValid() || !InventoryInterface)
	{
		return false;
	}

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int Quantity = Kv.Value * Multiplier;

		if (!InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
		{
			return false;
		}

		const FInventoryStack* Stack = GetInventoryStack(ContainerId, AssetId, InventoryInterface);
		const TArray<FInventoryRecord>* RecordList = GetRecords(ContainerId, Stack);
		if (!Stack && !RecordList)
		{
			return false;
		}

		bool bStackable = Stack->bStackable;
		if (bStackable)
		{
			if (!RecordList->IsValidIndex(0))
			{
				return false;
			}

			const FInventoryRecord& Record = (*RecordList)[0];
			if (Record.Quantity < Quantity)
			{
				return false;
			}

			continue;
		}

		if (RecordList->Num() < Quantity)
		{
			return false;
		}
	}

	return true;
}

bool UInventorySubsystem::ContainsItemById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, int Quantity) const
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
	{
		return false;
	}

	const TArray<FInventoryRecord>* RecordList = GetRecords(ContainerId, AssetId);
	if (!RecordList || !RecordList->IsValidIndex(0))
	{
		return false;
	}

	const FInventoryRecord* Record = RecordList->FindByPredicate([RecordId](const FInventoryRecord& Record) { return Record.ItemId == RecordId; });
	if (!Record)
	{
		return false;
	}

	return Record->Quantity >= Quantity;
}

int UInventorySubsystem::GetMaxMultiplier(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items) const
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!ContainerId.IsValid() || !InventoryInterface)
	{
		return 0;
	}

	int MaxMultiplier = INT_MAX;

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int Quantity = Kv.Value;

		if (!InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
		{
			continue;
		}

		int Available = GetTotalQuantity(ContainerId, AssetId, InventoryInterface);
		int PossibleMultiplier = Available / Quantity;

		MaxMultiplier = FMath::Max(MaxMultiplier, PossibleMultiplier);
	}

	return (MaxMultiplier == INT_MAX ? 0 : MaxMultiplier);
}


bool UInventorySubsystem::UpdateItem(FName ContainerId, const FPrimaryAssetId& AssetId, TFunctionRef<bool(FInventoryRecord*)> Callback)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId))
	{
		return false;
	}

	FInventoryRecord* Record = GetMutableRecord(ContainerId, AssetId);
	if (!Callback(Record))
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("Record update callback returned false"));
		return false;
	}

	OnItemUpdated.Broadcast(ContainerId, AssetId, Record->ItemId);
	return true;
}

bool UInventorySubsystem::UpdateItemById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, TFunctionRef<bool(FInventoryRecord*)> Callback)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId))
	{
		return false;
	}

	FInventoryRecord* Record = GetMutableRecordById(ContainerId, AssetId, RecordId);
	if (!Callback(Record))
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("Record update callback returned false"));
		return false;
	}

	OnItemUpdated.Broadcast(ContainerId, AssetId, Record->ItemId);
	return true;
}


int UInventorySubsystem::GetStackCount(FName ContainerId, const FPrimaryAssetId& AssetId) const
{
	return GetStackCount(ContainerId, AssetId, InventoryProvider.Get());
}

int UInventorySubsystem::GetStackCount(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface) const
{
	const TArray<FInventoryRecord>* RecordList = GetRecords(ContainerId, AssetId, InventoryInterface);
	if (!RecordList)
	{
		return 0;
	}

	return RecordList->Num();
}


int UInventorySubsystem::GetTotalQuantity(FName ContainerId, const FPrimaryAssetId& AssetId) const
{
	return GetTotalQuantity(ContainerId, AssetId, InventoryProvider.Get());
}

int UInventorySubsystem::GetTotalQuantity(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface) const
{
	const FInventoryStack* Stack = GetInventoryStack(ContainerId, AssetId, InventoryInterface);
	const TArray<FInventoryRecord>* RecordList = GetRecords(ContainerId, Stack);
	if (!Stack && !RecordList)
	{
		return 0;
	}

	bool bStackable = Stack->bStackable;
	if (bStackable)
	{
		if (!RecordList->IsValidIndex(0))
		{
			return 0;
		}

		const FInventoryRecord& Record = (*RecordList)[0];
		return Record.Quantity;
	}

	return RecordList->Num();
}


int UInventorySubsystem::GetQuantityById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId) const
{
	return GetQuantityById(ContainerId, AssetId, InventoryProvider.Get(), RecordId);
}

int UInventorySubsystem::GetQuantityById(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface, FName RecordId) const
{
	const TArray<FInventoryRecord>* RecordList = GetRecords(ContainerId, AssetId, InventoryInterface);
	if (!RecordList)
	{
		return 0;
	}

	const FInventoryRecord* Record = RecordList->FindByPredicate([RecordId](const FInventoryRecord& Record) { return Record.ItemId == RecordId; });
	if (!Record)
	{
		return 0;
	}

	return Record->Quantity;
}


const FInventoryStack* UInventorySubsystem::GetInventoryStack(FName ContainerId, const FPrimaryAssetId& AssetId) const
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	return GetInventoryStack(ContainerId, AssetId, InventoryInterface);
}

const FInventoryStack* UInventorySubsystem::GetInventoryStack(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface) const
{
	if (!InventoryInterface)
	{
		return nullptr;
	}

	const TMap<FName, FInventoryContainer>& Containers = InventoryInterface->GetInventoryContainer();
	const FInventoryContainer* Records = Containers.Find(ContainerId);
	if (!Records)
	{
		return nullptr;
	}

	FName ItemId = AssetId.PrimaryAssetName;
	const TMap<FName, FInventoryStack>& Stacks = Records->Stacks;
	const FInventoryStack* Stack = Stacks.Find(ItemId);
	if (!Stack)
	{
		return nullptr;
	}

	return Stack;
}


const TArray<FInventoryRecord>* UInventorySubsystem::GetRecords(FName ContainerId, const FPrimaryAssetId& AssetId) const
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	return GetRecords(ContainerId, AssetId, InventoryInterface);
}

const TArray<FInventoryRecord>* UInventorySubsystem::GetRecords(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface) const
{
	const FInventoryStack* Stack = GetInventoryStack(ContainerId, AssetId, InventoryInterface);
	return GetRecords(ContainerId, Stack);
}

const TArray<FInventoryRecord>* UInventorySubsystem::GetRecords(FName ContainerId, const FInventoryStack* Stack) const
{
	if (!Stack)
	{
		return nullptr;
	}
	const TArray<FInventoryRecord>& ItemRecords = Stack->ItemRecords;
	return &ItemRecords;
}


const FInventoryRecord* UInventorySubsystem::GetRecord(FName ContainerId, const FPrimaryAssetId& AssetId, int Index) const
{
	const TArray<FInventoryRecord>* RecordList = GetRecords(ContainerId, AssetId);
	if (!RecordList || !RecordList->IsValidIndex(Index))
	{
		return nullptr;
	}

	const FInventoryRecord& Record = (*RecordList)[Index];
	return &Record;
}

const FInventoryRecord* UInventorySubsystem::GetRecordById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId) const
{
	const TArray<FInventoryRecord>* RecordList = GetRecords(ContainerId, AssetId);
	if (!RecordList)
	{
		return nullptr;
	}

	const FInventoryRecord* Record = RecordList->FindByPredicate([RecordId](const FInventoryRecord& Record) { return Record.ItemId == RecordId; });
	return Record;
}


void UInventorySubsystem::QueryItems(UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback)
{
	if (QueryRule.QuerySource == EInventoryQuerySource::Glossary)
	{
		HandleGlossaryItems(FilterCriterion, QueryRule, MoveTemp(Callback));
	}
	else
	{
		HandleInventoryItems(FilterCriterion, QueryRule, MoveTemp(Callback));
	}
}


bool UInventorySubsystem::CreateContainer(FName ContainerId)
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!InventoryInterface || !ContainerId.IsValid())
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("InventoryInterface, ContainerId is invalid"));
		return false;
	}

	TMap<FName, FInventoryContainer>& Containers = InventoryProvider->GetMutableInventoryContainer();
	if (Containers.Contains(ContainerId))
	{
		PRINT_WARNING(LogInventory, 1.0f, TEXT("Container already exists"));
		return false;
	}

	Containers.Add(ContainerId, { FGuid::NewGuid() });
	OnContainerAdded.Broadcast(ContainerId);

	PRINT_INFO(LogInventory, 1.0f, TEXT("Container created"));
	return true;
}

bool UInventorySubsystem::RemoveContainer(FName ContainerId)
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!InventoryInterface || !ContainerId.IsValid())
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("InventoryProvider, ContainerId is invalid"));
		return false;
	}

	TMap<FName, FInventoryContainer>& Containers = InventoryInterface->GetMutableInventoryContainer();
	if (Containers.Remove(ContainerId) <= 0)
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("Container not found"));
		return false;
	}

	OnContainerRemoved.Broadcast(ContainerId);

	PRINT_INFO(LogInventory, 1.0f, TEXT("Container removed"));
	return true;
}


FInventoryStack* UInventorySubsystem::GetMutableInventoryStack(FName ContainerId, const FPrimaryAssetId& AssetId)
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	return GetMutableInventoryStack(ContainerId, AssetId, InventoryInterface);
}

FInventoryStack* UInventorySubsystem::GetMutableInventoryStack(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId) || !InventoryInterface)
	{
		return nullptr;
	}

	TMap<FName, FInventoryContainer>& Containers = InventoryInterface->GetMutableInventoryContainer();
	FInventoryContainer& Records = Containers.FindOrAdd(ContainerId, { FGuid::NewGuid() });

	FName ItemId = AssetId.PrimaryAssetName;
	TMap<FName, FInventoryStack>& Stacks = Records.Stacks;
	FInventoryStack* Stack = Stacks.Find(ItemId);
	if (Stack)
	{
		return Stack;
	}

	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
	{
		return nullptr;
	}

	bool bStackable = false;
	InventoryPrimaryAsset::GetStackable(AssetData, bStackable);

	bool bPersistWhenEmpty = false;
	InventoryPrimaryAsset::GetPersistWhenEmpty(AssetData, bPersistWhenEmpty);

	FInventoryStack& NewStack = Stacks.Add(ItemId, { FGuid::NewGuid(), bStackable, bPersistWhenEmpty });
	return &NewStack;
}


TArray<FInventoryRecord>* UInventorySubsystem::GetMutableRecords(FName InContainerId, const FPrimaryAssetId& InAssetId)
{
	return GetMutableRecords(InContainerId, InAssetId, InventoryProvider.Get());
}

TArray<FInventoryRecord>* UInventorySubsystem::GetMutableRecords(FName InContainerId, const FPrimaryAssetId& InAssetId, IInventoryProviderInterface* InventoryInterface)
{
	FInventoryStack* Stack = GetMutableInventoryStack(InContainerId, InAssetId, InventoryInterface);
	if (!Stack)
	{
		return nullptr;
	}
	return &Stack->ItemRecords;
}


FInventoryRecord* UInventorySubsystem::GetMutableRecord(FName ContainerId, const FPrimaryAssetId& AssetId, int Index)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId))
	{
		return nullptr;
	}

	TArray<FInventoryRecord>* RecordList = GetMutableRecords(ContainerId, AssetId);
	if (!RecordList || !RecordList->IsValidIndex(Index))
	{
		return nullptr;
	}

	FInventoryRecord& Record = (*RecordList)[Index];
	return &Record;
}

FInventoryRecord* UInventorySubsystem::GetMutableRecordById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId))
	{
		return nullptr;
	}

	TArray<FInventoryRecord>* RecordList = GetMutableRecords(ContainerId, AssetId);
	if (!RecordList)
	{
		return nullptr;
	}

	FInventoryRecord* Record = RecordList->FindByPredicate([RecordId](const FInventoryRecord& Record) { return Record.ItemId == RecordId; });
	return Record;
}


void UInventorySubsystem::HandleAddItem(const FPrimaryAssetId& AssetId, FInventoryRecord& Record)
{
	if (!AssetManager)
	{
		return;
	}

	UInventoryAsset* Asset = AssetManager->GetPrimaryAssetObject<UInventoryAsset>(AssetId);
	if (!Asset)
	{
		PRINT_WARNING(LogInventory, 1.0f, TEXT("Asset not loaded"));
		return;
	}

	const FInstancedStruct& Metadata = Asset->GetMetadata();
	if (!Metadata.IsValid())
	{
		PRINT_WARNING(LogInventory, 1.0f, TEXT("Asset metadata is invalid"));
		return;
	}

	Record.Metadata = Metadata;
}

void UInventorySubsystem::HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const
{
	const ESortDirection& SortDirection = QueryRule.SortDirection;
	const EInventorySortType& SortType = QueryRule.SortType;

	switch (SortType)
	{
	case EInventorySortType::None:
		LOG_WARNING(LogInventory, TEXT("No sort implemented"));
		break;
	case EInventorySortType::Alphabetical:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
				return (SortDirection == ESortDirection::Ascending) ?
					A.ItemName.ToString() < B.ItemName.ToString() :
					A.ItemName.ToString() > B.ItemName.ToString();
			}
		);
		break;
	case EInventorySortType::Quantity:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
				return (SortDirection == ESortDirection::Ascending) ?
					A.ItemQuantity < B.ItemQuantity :
					A.ItemQuantity > B.ItemQuantity;
			}
		);
		break;
	case EInventorySortType::Level:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
				if (!A.Record || !B.Record) return false;
				return (SortDirection == ESortDirection::Ascending) ?
					A.Record->Enhancement.Level < B.Record->Enhancement.Level :
					A.Record->Enhancement.Level > B.Record->Enhancement.Level;
			}
		);
		break;
	case EInventorySortType::Rank:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
				if (!A.Record || !B.Record) return false;
				return (SortDirection == ESortDirection::Ascending) ?
					A.Record->Enhancement.Rank < B.Record->Enhancement.Rank :
					A.Record->Enhancement.Rank > B.Record->Enhancement.Rank;
			}
		);
		break;
	default:
		LOG_WARNING(LogInventory, TEXT("No sort implemented"));
		break;
	}
}

void UInventorySubsystem::HandleGlossaryItems(UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback) const
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!IsValid(AssetManager) || !InventoryInterface)
	{
		LOG_ERROR(LogInventory, TEXT("AssetManager, InventoryInterface is invalid"));
		return;
	}

	bool bFilterValid = IsValid(FilterCriterion);
	const FName& ContainerId = QueryRule.ContainerId;

	TArray<FPrimaryAssetId> AssetIds;
	AssetManager->GetPrimaryAssetIdList(InventoryPrimaryAsset::GetAssetType(), AssetIds);

	TArray<FInventorySortEntry> SortedItems;
	SortedItems.Reserve(AssetIds.Num());

	for (const FPrimaryAssetId& AssetId : AssetIds)
	{
		FAssetData AssetData;
		if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
		{
			continue;
		}

		FName ItemId = AssetId.PrimaryAssetName;
		FText DisplayName = FText::GetEmpty();
		InventoryPrimaryAsset::GetDisplayName(AssetData, DisplayName);

		FName ItemType = NAME_None;
		InventoryPrimaryAsset::GetType(AssetData, ItemType);

		FName ItemRarity = NAME_None;
		InventoryPrimaryAsset::GetRarity(AssetData, ItemRarity);

		bool bStackable = false;
		InventoryPrimaryAsset::GetStackable(AssetData, bStackable);

		if (bFilterValid)
		{
			FFilterContext Context;
			Context.SetValue(InventoryFilterProperty::AssetId, AssetId);
			Context.SetValue(InventoryFilterProperty::AssetType, ItemType);
			Context.SetValue(InventoryFilterProperty::AssetRarity, ItemRarity);

			if (!FilterCriterion->Evaluate(Context))
			{
				continue;
			}
		}

		int ItemQuantity = 0;
		if (bStackable)
		{
			ItemQuantity = GetTotalQuantity(ContainerId, AssetId, InventoryInterface);
		}
		else
		{
			ItemQuantity = GetStackCount(ContainerId, AssetId, InventoryInterface);
		}

		SortedItems.Emplace(AssetId, DisplayName, ItemQuantity);
	}

	HandleItemSorting(SortedItems, QueryRule);

	for (const FInventorySortEntry& Item : SortedItems)
	{
		Callback(Item);
	}
}

void UInventorySubsystem::HandleInventoryItems(UFilterCriterion* FilterCriterion, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> Callback) const
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!InventoryInterface)
	{
		return;
	}

	const FName& ContainerId = QueryRule.ContainerId;

	const TMap<FName, FInventoryContainer>& Containers = InventoryInterface->GetInventoryContainer();
	const FInventoryContainer* Container = Containers.Find(ContainerId);
	if (!Container)
	{
		return;
	}

	bool bFilterValid = IsValid(FilterCriterion);
	TArray<FInventorySortEntry> SortedItems;

	const TMap<FName, FInventoryStack>& Stacks = Container->Stacks;
	for (const TPair<FName, FInventoryStack>& Kv : Stacks)
	{
		FName ItemId = Kv.Key;
		const FInventoryStack* Stack = &Kv.Value;
		if (!Stack)
		{
			continue;
		}

		FPrimaryAssetId AssetId = InventoryPrimaryAsset::GetPrimaryAssetId(ItemId);
		FAssetData AssetData;
		if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
		{
			continue;
		}

		FText DisplayName = FText::GetEmpty();
		bool bNameValid = InventoryPrimaryAsset::GetDisplayName(AssetData, DisplayName);

		FName ItemType = TEXT_EMPTY;
		bool bTypeValid = InventoryPrimaryAsset::GetType(AssetData, ItemType);

		FName ItemRarity = TEXT_EMPTY;
		bool bRarityValid = InventoryPrimaryAsset::GetRarity(AssetData, ItemRarity);

		if (!bNameValid || !bTypeValid || !bRarityValid)
		{
			continue;
		}

		const TArray<FInventoryRecord>& Records = Stack->ItemRecords;
		for (const FInventoryRecord& Record : Records)
		{
			int ItemQuantity = Record.Quantity;

			if (bFilterValid)
			{
				FFilterContext Context;
				Context.SetValue(InventoryFilterProperty::AssetId, AssetId);
				Context.SetValue(InventoryFilterProperty::AssetType, ItemType);
				Context.SetValue(InventoryFilterProperty::AssetRarity, ItemRarity);
				Context.SetValue(InventoryFilterProperty::ItemId, Record.ItemId);
				Context.SetValue(InventoryFilterProperty::ItemQuantity, ItemQuantity);

				if (!FilterCriterion->Evaluate(Context))
				{
					continue;
				}
			}

			SortedItems.Emplace(AssetId, DisplayName, ItemQuantity, &Record);
		}
	}

	HandleItemSorting(SortedItems, QueryRule);

	for (const FInventorySortEntry& Item : SortedItems)
	{
		Callback(Item);
	}
}


void UInventorySubsystem::HandleStorageLoaded()
{
	FLatentDelegates::OnStorageLoaded.RemoveAll(this);
	LOG_INFO(LogInventory, TEXT("Storage loading"));

	IInventoryProviderInterface* InventoryProviderInterface = StorageUtils::GetStorageInterface<IInventoryProviderInterface>(GetGameInstance());
	InventoryProvider = TWeakInterfacePtr<IInventoryProviderInterface>(InventoryProviderInterface);

	AssetManager = UAssetManager::GetIfInitialized();

	LOG_INFO(LogInventory, TEXT("Storage loaded"));
}


bool UInventorySubsystem::AddItemRecord(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack)
{
	if (!InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0 || !Stack)
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("AssetId, Stack is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TArray<FInventoryRecord>& RecordList = Stack->ItemRecords;
	bool bStackable = Stack->bStackable;
	if (bStackable)
	{
		if (RecordList.IsValidIndex(0))
		{
			FInventoryRecord& Record = RecordList[0];
			AddItemRecord_Internal(ContainerId, AssetId, Record, Quantity);
			return true;
		}
		else
		{
			AddItemRecord_Internal(ContainerId, AssetId, RecordList, Quantity);
			return true;
		}
	}

	for (int i = 0; i < Quantity; i++)
	{
		AddItemRecord_Internal(ContainerId, AssetId, RecordList, 1);
	}
	
	return true;
}

void UInventorySubsystem::AddItemRecord_Internal(FName ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryRecord>& RecordList, int Quantity)
{
	FName RecordId = FName(FGuid::NewGuid().ToString());

	FInventoryRecord Record;
	Record.ItemId = RecordId;
	Record.Quantity = Quantity;

	HandleAddItem(AssetId, Record);

	RecordList.Add(Record);
	PRINT_INFO(LogInventory, 1.0f, TEXT("New record added"));

	OnItemAdded.Broadcast(ContainerId, AssetId, RecordId);
}

void UInventorySubsystem::AddItemRecord_Internal(FName ContainerId, const FPrimaryAssetId& AssetId, FInventoryRecord& Record, int Quantity)
{
	Record.Quantity += Quantity;
	PRINT_INFO(LogInventory, 1.0f, TEXT("Record updated"));

	OnItemAdded.Broadcast(ContainerId, AssetId, Record.ItemId);
}


bool UInventorySubsystem::RemoveItemRecord(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, FInventoryStack* Stack)
{
	if (!InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0 || !Stack)
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("AssetId, Stack is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TArray<FInventoryRecord>& RecordList = Stack->ItemRecords;
	bool bStackable = Stack->bStackable;
	if (bStackable)
	{
		if (RecordList.IsValidIndex(0))
		{
			bool bPersistWhenEmpty = Stack->bPersistWhenEmpty;

			FInventoryRecord& Record = RecordList[0];
			RemoveItemRecord_Internal(ContainerId, AssetId, &RecordList, bPersistWhenEmpty, &Record, Quantity);

			return true;
		}

		PRINT_ERROR(LogInventory, 1.0f, TEXT("Not item to remove"));
		return false;
	}

	bool bRemoved = RemoveItemRecord_Internal(ContainerId, AssetId, &RecordList, Quantity);
	return bRemoved;
}

bool UInventorySubsystem::RemoveItemRecordById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, int Quantity, FInventoryStack* Stack)
{
	if (!InventoryPrimaryAsset::IsValid(AssetId) || !RecordId.IsValid() || Quantity <= 0 || !Stack)
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("AssetId, RecordId, Stack is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TArray<FInventoryRecord>& RecordList = Stack->ItemRecords;

	FInventoryRecord* Record = RecordList.FindByPredicate([RecordId](const FInventoryRecord& Record) { return Record.ItemId == RecordId; });
	if (!Record)
	{
		PRINT_ERROR(LogInventory, 1.0f, TEXT("Record not found for InRecordId"));
		return false;
	}

	bool bStackable = Stack->bStackable;
	if (bStackable)
	{
		bool bPersistWhenEmpty = Stack->bPersistWhenEmpty;
		RemoveItemRecord_Internal(ContainerId, AssetId, &RecordList, bPersistWhenEmpty, Record, Quantity);
		return true;
	}

	bool bRemoved = RemoveItemRecord_Internal(ContainerId, AssetId, &RecordList, Record);
	return bRemoved;
}


bool UInventorySubsystem::RemoveItemRecord_Internal(FName ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryRecord>* RecordList, int Quantity)
{
	int StackCount = RecordList->Num();
	if (Quantity > StackCount)
	{
		return false;
	}

	for (int i = 0; i < Quantity; i++)
	{
		FInventoryRecord Record = RecordList->Pop();
		OnItemRemoved.Broadcast(ContainerId, AssetId, Record.ItemId);
	}
	return true;
}

bool UInventorySubsystem::RemoveItemRecord_Internal(FName ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryRecord>* RecordList, FInventoryRecord* Record)
{
	FInventoryRecord RecordCopy = *Record;

	int Count = RecordList->Remove(RecordCopy);
	if (Count == 0)
	{
		return false;
	}

	OnItemRemoved.Broadcast(ContainerId, AssetId, RecordCopy.ItemId);
	return true;
}

void UInventorySubsystem::RemoveItemRecord_Internal(FName ContainerId, const FPrimaryAssetId& AssetId, TArray<FInventoryRecord>* RecordList, bool bPersistWhenEmpty, FInventoryRecord* Record, int Quantity)
{
	FName ItemId = Record->ItemId;

	int NewQuantity = Record->Quantity - Quantity;
	if (NewQuantity > 0)
	{
		Record->Quantity = NewQuantity;
		OnItemRemoved.Broadcast(ContainerId, AssetId, ItemId);
		return;
	}

	if (bPersistWhenEmpty)
	{
		Record->Quantity = 0;
		OnItemRemoved.Broadcast(ContainerId, AssetId, ItemId);
		return;
	}

	RemoveItemRecord_Internal(ContainerId, AssetId, RecordList, Record);
}


bool UInventorySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogInventory, TEXT("Initialized"));

	FLatentDelegates::OnStorageLoaded.AddUObject(this, &UInventorySubsystem::HandleStorageLoaded);
}

void UInventorySubsystem::Deinitialize()
{
	FLatentDelegates::OnStorageLoaded.RemoveAll(this);

	InventoryProvider.Reset();
	AssetManager = nullptr;

	LOG_WARNING(LogInventory, TEXT("Deinitialized"));
	Super::Deinitialize();
}

