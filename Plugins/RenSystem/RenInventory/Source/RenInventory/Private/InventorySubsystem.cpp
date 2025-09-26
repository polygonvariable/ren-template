// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InventorySubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "RenAsset/Public/PrimaryAssetMap.h"

#include "RCoreDelegate/Public/LatentDelegates.h"
#include "RCoreSettings/Public/GameMetadataSettings.h"

#include "RCoreStorage/Public/StorageProviderInterface.h"

#include "RCoreInventory/Public/InventoryAsset.h"
#include "RCoreInventory/Public/InventoryItemType.h"
#include "RCoreInventory/Public/InventoryProviderInterface.h"
#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreLibrary/Public/SubsystemUtils.h"

#include "RenInventory/Public/InventoryPrimaryAsset.h"


/*
bool UInventorySubsystem::AddItem(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity)
{
	if (!IsValid(AssetManager) || !ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("AssetManager, ContainerId or AssetId is invalid or Quantity is less than or equal to 0"));
		return false;
	}

	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container"));
		return false;
	}

	FName RecordId = TEXT_EMPTY;
	const FInventoryRecord* Record = AddItemRecord(AssetId, Quantity, Records, RecordId);

	if (Record)
	{
		OnItemAdded.Broadcast(ContainerId, RecordId, Record);
		return true;
	}

	return false;
}

bool UInventorySubsystem::AddItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items)
{
	if (!IsValid(AssetManager) || !ContainerId.IsValid())
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("AssetManager, ContainerId or AssetId is invalid"));
		return false;
	}

	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container"));
		return false;
	}

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId AssetId = Kv.Key;
		int Quantity = Kv.Value;

		if (InventoryPrimaryAsset::IsValid(AssetId) && Quantity > 0)
		{
			FName RecordId = TEXT_EMPTY;
			const FInventoryRecord* Record = AddItemRecord(AssetId, Quantity, Records, RecordId);

			if (Record)
			{
				OnItemAdded.Broadcast(ContainerId, RecordId, Record);
			}
		}
	}

	return true;
}

bool UInventorySubsystem::RemoveItem(FName ContainerId, FName RecordId, int Quantity)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container"));
		return false;
	}

	bool bRemoved = false;
	const FInventoryRecord* Record = RemoveItemRecord(RecordId, Quantity, Records, bRemoved);
	if (bRemoved)
	{
		OnItemRemoved.Broadcast(ContainerId, RecordId, Record);
		return true;
	}

	return false;
}

bool UInventorySubsystem::RemoveItems(FName ContainerId, const TMap<FName, int>& ItemQuantities)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container"));
		return false;
	}

	for (const TPair<FName, int>& Kv : ItemQuantities)
	{
		const FName& RecordId = Kv.Key;
		int Quantity = Kv.Value;

		if (RecordId.IsValid() && Quantity > 0)
		{
			bool bRemoved = false;
			const FInventoryRecord* Record = RemoveItemRecord(RecordId, Quantity, Records, bRemoved);
			if (bRemoved)
			{
				OnItemRemoved.Broadcast(ContainerId, RecordId, Record);
			}
		}
	}

	return true;
}

bool UInventorySubsystem::RemoveItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& ItemQuantities)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container"));
		return false;
	}

	for (const TPair<FPrimaryAssetId, int>& Kv : ItemQuantities)
	{
		const FName& RecordId = Kv.Key.PrimaryAssetName;
		int Quantity = Kv.Value;

		if (RecordId.IsValid() && Quantity > 0)
		{
			bool bRemoved = false;
			const FInventoryRecord* Record = RemoveItemRecord(RecordId, Quantity, Records, bRemoved);
			if (bRemoved)
			{
				OnItemRemoved.Broadcast(ContainerId, RecordId, Record);
			}
		}
	}

	return true;
}

bool UInventorySubsystem::DeleteItem(FName ContainerId, FName RecordId)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container"));
		return false;
	}

	if (Records->Remove(RecordId) <= 0)
	{
		PRINT_WARNING(LogTemp, 1.0f, TEXT("No item deleted"));
		return false;
	}

	PRINT_INFO(LogTemp, 1.0f, TEXT("Item deleted"));
	return true;
}

int UInventorySubsystem::DeleteItems(FName ContainerId, const TArray<FName>& RecordIds)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container"));
		return 0;
	}

	int Count = 0;
	for (const FName& RecordId : RecordIds)
	{
		Count += Records->Remove(RecordId);
	}

	PRINT_INFO(LogTemp, 1.0f, TEXT("Items deleted"));
	return Count;
}

bool UInventorySubsystem::ClearItems(FName ContainerId)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container"));
		return false;
	}

	Records->Empty();

	PRINT_INFO(LogTemp, 1.0f, TEXT("Items cleared"));
	return true;
}

bool UInventorySubsystem::ReplaceItem(FName ContainerId, FName RecordId, FInventoryRecord Record)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container"));
		return false;
	}

	Records->Add(RecordId, Record);

	PRINT_INFO(LogTemp, 1.0f, TEXT("Record replaced"));
	return true;
}


int UInventorySubsystem::CountItem(FName ContainerId, FName ItemId) const
{
	const TMap<FName, FInventoryRecord>* Records = GetRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container"));
		return 0;
	}

	int Count = 0;
	for (const TPair<FName, FInventoryRecord>& Kv : *Records)
	{
		if (Kv.Value.ItemId == ItemId)
		{
			Count++;
		}
	}

	return Count;
}

bool UInventorySubsystem::UpdateItem(FName ContainerId, FName RecordId, TFunctionRef<bool(FInventoryRecord*)> InCallback)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container"));
		return false;
	}

	FInventoryRecord* Record = Records->Find(RecordId);
	if (!Record)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Record not found"));
		return false;
	}

	if (!InCallback(Record))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Record update callback returned false"));
		return false;
	}

	OnItemUpdated.Broadcast(ContainerId, RecordId, Record);
	return true;
}

bool UInventorySubsystem::ContainsItem(FName ContainerId, FName RecordId, int Quantity) const
{
	const TMap<FName, FInventoryRecord>* Records = GetRecords(ContainerId);
	if (!Records)
	{
		return false;
	}

	const FInventoryRecord* Record = Records->Find(RecordId);
	if (!Record)
	{
		return false;
	}

	return Record->ItemQuantity >= Quantity;
}

bool UInventorySubsystem::ContainsItem(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity) const
{
	return ContainsItem(ContainerId, AssetId.PrimaryAssetName, Quantity);
}

bool UInventorySubsystem::ContainsItems(FName ContainerId, const TMap<FName, int>& Items) const
{
	const TMap<FName, FInventoryRecord>* Records = GetRecords(ContainerId);
	if (!Records)
	{
		return false;
	}

	for (const TPair<FName, int>& Kv : Items)
	{
		const FInventoryRecord* Record = Records->Find(Kv.Key);
		if (!Record || Record->ItemQuantity < Kv.Value)
		{
			return false;
		}
	}

	return true;
}

bool UInventorySubsystem::ContainsItems(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items) const
{
	const TMap<FName, FInventoryRecord>* Records = GetRecords(ContainerId);
	if (!Records)
	{
		return false;
	}

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FInventoryRecord* Record = Records->Find(Kv.Key.PrimaryAssetName);
		if (!Record || Record->ItemQuantity < Kv.Value)
		{
			return false;
		}
	}

	return true;
}

TMap<FName, FInventoryRecord>* UInventorySubsystem::GetMutableRecords(FName ContainerId) const
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!InventoryInterface)
	{
		return nullptr;
	}

	TMap<FName, FInventoryContainer>& Containers = InventoryInterface->GetMutableInventoryContainer();
	FInventoryContainer* Records = Containers.Find(ContainerId);

	if (!Records)
	{
		return nullptr;
	}

	return &Records->Items;
}

const TMap<FName, FInventoryRecord>* UInventorySubsystem::GetRecords(FName ContainerId) const
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
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
	
	return &Records->Items;
}


FInventoryRecord* UInventorySubsystem::GetMutableItemRecord(FName ContainerId, FName RecordId) const
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		return nullptr;
	}
	return Records->Find(RecordId);
}

const FInventoryRecord* UInventorySubsystem::GetItemRecord(FName ContainerId, FName RecordId) const
{
	const TMap<FName, FInventoryRecord>* Records = GetRecords(ContainerId);
	if (!Records)
	{
		return nullptr;
	}
	return Records->Find(RecordId);
}

void UInventorySubsystem::QueryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FPrimaryAssetId&, const FName&, const FInventoryRecord*)> InCallback) const
{
	if (QueryRule.QuerySource == EInventoryQuerySource::Glossary)
	{
		HandleGlossaryItems(FilterRule, QueryRule, InCallback);
	}
	else
	{
		HandleInventoryItems(FilterRule, QueryRule, InCallback);
	}
}

void UInventorySubsystem::HandleGlossaryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FPrimaryAssetId&, const FName&, const FInventoryRecord*)> InCallback) const
{
	if (!IsValid(AssetManager))
	{
		LOG_ERROR(LogTemp, TEXT("AssetManager is invalid"));
		return;
	}

	const FName& ContainerId = QueryRule.ContainerId;

	TArray<FPrimaryAssetId> AssetIds;
	AssetManager->GetPrimaryAssetIdList(InventoryPrimaryAsset::GetAssetType(), AssetIds);

	if (QueryRule.SortType != EInventorySortType::None)
	{
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
			FText ItemName = FText::GetEmpty();
			bool bNameValid = InventoryPrimaryAsset::GetItemName(AssetData, ItemName);

			FName ItemType = TEXT_EMPTY;
			bool bTypeValid = InventoryPrimaryAsset::GetItemType(AssetData, ItemType);

			FName ItemRarity = TEXT_EMPTY;
			bool bRarityValid = InventoryPrimaryAsset::GetItemRarity(AssetData, ItemRarity);

			const FInventoryRecord* Record = GetItemRecord(ContainerId, ItemId);

			if (!FilterRule.Match(Record, ItemId, ItemType, ItemRarity))
			{
				continue;
			}

			SortedItems.Emplace(AssetId, ItemName, ItemId, Record);
		}

		HandleItemSorting(SortedItems, QueryRule);

		for (const FInventorySortEntry& Item : SortedItems)
		{
			InCallback(Item.AssetId, Item.RecordId, Item.Record);
		}
	}
	else
	{
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

			FName ItemType = TEXT_EMPTY;
			bool bTypeValid = InventoryPrimaryAsset::GetItemType(AssetData, ItemType);

			FName ItemRarity = TEXT_EMPTY;
			bool bRarityValid = InventoryPrimaryAsset::GetItemRarity(AssetData, ItemRarity);

			const FInventoryRecord* Record = GetItemRecord(ContainerId, ItemId);

			if (!FilterRule.Match(Record, ItemId, ItemType, ItemRarity))
			{
				continue;
			}

			InCallback(AssetId, ItemId, Record);
		}
	}
}

void UInventorySubsystem::HandleInventoryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FPrimaryAssetId&, const FName&, const FInventoryRecord*)> InCallback) const
{
	if (!IsValid(AssetManager))
	{
		LOG_ERROR(LogTemp, TEXT("AssetManager is invalid"));
		return;
	}

	const FName& ContainerId = QueryRule.ContainerId;

	const TMap<FName, FInventoryRecord>* Records = GetRecords(ContainerId);
	if (!Records)
	{
		LOG_ERROR(LogTemp, TEXT("Records not found in container"));
		return;
	}

	if (QueryRule.SortType != EInventorySortType::None)
	{
		TArray<FInventorySortEntry> SortedItems;
		SortedItems.Reserve(Records->Num());

		for (const TPair<FName, FInventoryRecord>& Kv : *Records)
		{
			FName RecordId = Kv.Key;
			const FInventoryRecord& Record = Kv.Value;

			FPrimaryAssetId AssetId = InventoryPrimaryAsset::GetPrimaryAssetId(Record.ItemId);
			FAssetData AssetData;
			if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
			{
				continue;
			}

			FText ItemName = FText::GetEmpty();
			bool bNameValid = InventoryPrimaryAsset::GetItemName(AssetData, ItemName);

			FName ItemType = TEXT_EMPTY;
			bool bTypeValid = InventoryPrimaryAsset::GetItemType(AssetData, ItemType);

			FName ItemRarity = TEXT_EMPTY;
			bool bRarityValid = InventoryPrimaryAsset::GetItemRarity(AssetData, ItemRarity);

			if (!bNameValid || !bTypeValid || !bRarityValid)
			{
				continue;
			}

			if (!FilterRule.Match(&Record, Record.ItemId, ItemType, ItemRarity))
			{
				continue;
			}

			SortedItems.Emplace(AssetId, ItemName, RecordId, &Record);
		}

		HandleItemSorting(SortedItems, QueryRule);

		for (const auto& Item : SortedItems)
		{
			InCallback(Item.AssetId, Item.RecordId, Item.Record);
		}
	}
	else
	{
		for (const TPair<FName, FInventoryRecord>& Kv : *Records)
		{
			FName RecordId = Kv.Key;
			const FInventoryRecord& Record = Kv.Value;

			FPrimaryAssetId AssetId = InventoryPrimaryAsset::GetPrimaryAssetId(Record.ItemId);
			FAssetData AssetData;
			if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
			{
				continue;
			}

			FName ItemType = TEXT_EMPTY;
			bool bTypeValid = InventoryPrimaryAsset::GetItemType(AssetData, ItemType);

			FName ItemRarity = TEXT_EMPTY;
			bool bRarityValid = InventoryPrimaryAsset::GetItemRarity(AssetData, ItemRarity);

			if (!bTypeValid || !bRarityValid)
			{
				continue;
			}

			if (!FilterRule.Match(&Record, Record.ItemId, ItemType, ItemRarity))
			{
				continue;
			}

			InCallback(AssetId, RecordId, &Record);
		}
	}
}


const FInventoryRecord* UInventorySubsystem::AddItemRecord(const FPrimaryAssetId& AssetId, int Quantity, TMap<FName, FInventoryRecord>* Records, FName& OutRecordId)
{
	if (!AssetId.IsValid() || Quantity <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("AssetId is invalid or Quantity less than or equal to 0"));

		OutRecordId = TEXT_EMPTY;
		return nullptr;
	}
	
	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("AssetData not found for AssetId"));

		OutRecordId = TEXT_EMPTY;
		return nullptr;
	}

	bool bIsStackable = false;
	InventoryPrimaryAsset::GetItemIsStackable(AssetData, bIsStackable);

	FName ItemId = AssetId.PrimaryAssetName;
	FName RecordId = ItemId;
	
	if (bIsStackable)
	{
		FInventoryRecord* Record = Records->Find(ItemId);
		if (Record)
		{
			Record->ItemQuantity += Quantity;

			PRINT_INFO(LogTemp, 1.0f, TEXT("Record updated"));

			OutRecordId = RecordId;
			return Record;
		}
	}
	else
	{
		RecordId = FName(FGuid::NewGuid().ToString());
	}

	EInventoryItemType ItemType;
	if (!InventoryPrimaryAsset::GetItemType(AssetData, ItemType))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item type not found"));

		OutRecordId = TEXT_EMPTY;
		return nullptr;
	}

	FInventoryRecord& NewRecord = Records->Add(RecordId, { ItemId, ItemType, Quantity });

	PRINT_INFO(LogTemp, 1.0f, TEXT("New record added"));

	OutRecordId = RecordId;
	return &NewRecord;
}

const FInventoryRecord* UInventorySubsystem::RemoveItemRecord(FName RecordId, int Quantity, TMap<FName, FInventoryRecord>* Records, bool bOutRemoved)
{
	if (RecordId.IsNone() || Quantity <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Quantity less than or equal to 0"));

		bOutRemoved = false;
		return nullptr;
	}

	FInventoryRecord* Record = Records->Find(RecordId);
	if (!Record)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Record not found"));

		bOutRemoved = false;
		return nullptr;
	}

	int FinalQuantity = Record->ItemQuantity - Quantity;
	if (FinalQuantity > 0)
	{
		Record->ItemQuantity = FinalQuantity;
		PRINT_INFO(LogTemp, 1.0f, TEXT("Record quantity reduced"));

		bOutRemoved = true;
		return Record;
	}

	FPrimaryAssetId AssetId = InventoryPrimaryAsset::GetPrimaryAssetId(Record->ItemId);
	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("AssetData not found for AssetId"));

		bOutRemoved = false;
		return nullptr;
	}

	bool bAllowEmptyData = false;
	InventoryPrimaryAsset::GetItemAllowEmptyData(AssetData, bAllowEmptyData);

	if (bAllowEmptyData)
	{
		Record->ItemQuantity = 0;
		PRINT_INFO(LogTemp, 1.0f, TEXT("Record quantity reduced to zero"));

		bOutRemoved = true;
		return Record;
	}

	Records->Remove(RecordId);
	PRINT_INFO(LogTemp, 1.0f, TEXT("Record removed"));

	bOutRemoved = true;
	return nullptr;
}

*/



void UInventorySubsystem::HandleItemSorting(TArray<FInventorySortEntry>& SortedItems, const FInventoryQueryRule& QueryRule) const
{
	const ESortDirection& SortDirection = QueryRule.SortDirection;
	const EInventorySortType& SortType = QueryRule.SortType;

	switch (SortType)
	{
	case EInventorySortType::None:
		LOG_WARNING(LogTemp, TEXT("No sort implemented"));
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
					A.Record->EnhanceRecord.Level < B.Record->EnhanceRecord.Level :
					A.Record->EnhanceRecord.Level > B.Record->EnhanceRecord.Level;
			}
		);
		break;
	case EInventorySortType::Rank:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
				if (!A.Record || !B.Record) return false;
				return (SortDirection == ESortDirection::Ascending) ?
					A.Record->EnhanceRecord.Rank < B.Record->EnhanceRecord.Rank :
					A.Record->EnhanceRecord.Rank > B.Record->EnhanceRecord.Rank;
			}
		);
		break;
	default:
		LOG_WARNING(LogTemp, TEXT("No sort implemented"));
		break;
	}
}

bool UInventorySubsystem::CreateContainer(FName ContainerId)
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!InventoryInterface)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid"));
		return false;
	}

	TMap<FName, FInventoryContainer>& Containers = InventoryProvider->GetMutableInventoryContainer();
	if (Containers.Contains(ContainerId))
	{
		PRINT_WARNING(LogTemp, 1.0f, TEXT("Container already exists"));
		return false;
	}

	Containers.Add(ContainerId, { FGuid::NewGuid() });
	OnContainerAdded.Broadcast(ContainerId);

	PRINT_INFO(LogTemp, 1.0f, TEXT("Container created"));
	return true;
}

bool UInventorySubsystem::RemoveContainer(FName ContainerId)
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!InventoryInterface)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryProvider is invalid"));
		return false;
	}

	TMap<FName, FInventoryContainer>& Containers = InventoryInterface->GetMutableInventoryContainer();
	if (Containers.Remove(ContainerId) <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Container not found"));
		return false;
	}

	OnContainerRemoved.Broadcast(ContainerId);

	PRINT_INFO(LogTemp, 1.0f, TEXT("Container removed"));
	return true;
}


void UInventorySubsystem::HandleStorageLoaded()
{
	FLatentDelegates::OnStorageLoaded.RemoveAll(this);
	LOG_INFO(LogTemp, TEXT("InventorySubsystem storage load started"));

	IStorageProviderInterface* StorageProvider = SubsystemUtils::GetSubsystemInterface<UGameInstance, UGameInstanceSubsystem, IStorageProviderInterface>(GetGameInstance());
	if (!StorageProvider)
	{
		LOG_ERROR(LogTemp, TEXT("StorageProvider is invalid"));
		return;
	}

	UObject* Storage = StorageProvider->GetLocalStorage();
	IInventoryProviderInterface* InventoryProviderInterface = Cast<IInventoryProviderInterface>(Storage);
	if (!InventoryProviderInterface)
	{
		LOG_ERROR(LogTemp, TEXT("InventoryInterface is invalid"));
		return;
	}

	InventoryProvider = TWeakInterfacePtr<IInventoryProviderInterface>(InventoryProviderInterface);
	LOG_INFO(LogTemp, TEXT("InventorySubsystem storage loaded"));
}

bool UInventorySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("InventorySubsystem initialized"));

	AssetManager = UAssetManager::GetIfInitialized();

	if (!FLatentDelegates::OnStorageLoaded.IsBoundToObject(this))
	{
		FLatentDelegates::OnStorageLoaded.AddUObject(this, &UInventorySubsystem::HandleStorageLoaded);
	}
}

void UInventorySubsystem::Deinitialize()
{
	InventoryProvider.Reset();

	AssetManager = nullptr;

	FLatentDelegates::OnStorageLoaded.RemoveAll(this);

	LOG_WARNING(LogTemp, TEXT("InventorySubsystem deinitialized"));
	Super::Deinitialize();
}











const TArray<FInventoryRecord>* UInventorySubsystem::GetItemStack(FName ContainerId, FName ItemId) const
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	return GetItemStack(ContainerId, ItemId, InventoryInterface);
}

const TArray<FInventoryRecord>* UInventorySubsystem::GetItemStack(FName ContainerId, FName ItemId, IInventoryProviderInterface* InventoryInterface) const
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

	const TMap<FName, FInventoryStack>& Stacks = Records->ItemStacks;
	const FInventoryStack* InventoryStack = Stacks.Find(ItemId);
	if (!InventoryStack)
	{
		return nullptr;
	}

	return &InventoryStack->Records;
}

TArray<FInventoryRecord>* UInventorySubsystem::GetMutableItemStack(FName ContainerId, FName ItemId)
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	return GetMutableItemStack(ContainerId, ItemId, InventoryInterface);
}

TArray<FInventoryRecord>* UInventorySubsystem::GetMutableItemStack(FName ContainerId, FName ItemId, IInventoryProviderInterface* InventoryInterface)
{
	if (!InventoryInterface)
	{
		return nullptr;
	}

	TMap<FName, FInventoryContainer>& Containers = InventoryInterface->GetMutableInventoryContainer();
	FInventoryContainer* Records = Containers.Find(ContainerId);
	if (!Records)
	{
		return nullptr;
	}

	TMap<FName, FInventoryStack>& Stacks = Records->ItemStacks;
	FInventoryStack& InventoryStack = Stacks.FindOrAdd(ItemId, { FGuid::NewGuid() });
	return &InventoryStack.Records;
}


const FInventoryRecord* UInventorySubsystem::GetRecordByIndex(FName ContainerId, const FPrimaryAssetId& AssetId, int Index) const
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId))
	{
		return nullptr;
	}

	FName ItemId = AssetId.PrimaryAssetName;
	const TArray<FInventoryRecord>* ItemStack = GetItemStack(ContainerId, ItemId);
	if (!ItemStack || !ItemStack->IsValidIndex(Index))
	{
		return nullptr;
	}

	const FInventoryRecord& Record = (*ItemStack)[Index];
	return &Record;
}

const FInventoryRecord* UInventorySubsystem::GetRecordById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId) const
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId))
	{
		return nullptr;
	}

	FName ItemId = AssetId.PrimaryAssetName;
	const TArray<FInventoryRecord>* ItemStack = GetItemStack(ContainerId, ItemId);
	if (!ItemStack)
	{
		return nullptr;
	}

	const FInventoryRecord* Record = ItemStack->FindByPredicate([RecordId](const FInventoryRecord& Record) { return Record.ItemId == RecordId; });
	return Record;
}

FInventoryRecord* UInventorySubsystem::GetMutableRecordById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId))
	{
		return nullptr;
	}

	FName ItemId = AssetId.PrimaryAssetName;
	TArray<FInventoryRecord>* ItemStack = GetMutableItemStack(ContainerId, ItemId);
	if (!ItemStack)
	{
		return nullptr;
	}

	FInventoryRecord* Record = ItemStack->FindByPredicate([RecordId](const FInventoryRecord& Record) { return Record.ItemId == RecordId; });
	return Record;
}


FInventoryRecord* UInventorySubsystem::GetMutableRecordByIndex(FName ContainerId, const FPrimaryAssetId& AssetId, int Index)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId))
	{
		return nullptr;
	}

	FName ItemId = AssetId.PrimaryAssetName;
	TArray<FInventoryRecord>* ItemStack = GetMutableItemStack(ContainerId, ItemId);
	if (!ItemStack || !ItemStack->IsValidIndex(Index))
	{
		return nullptr;
	}

	FInventoryRecord& Record = (*ItemStack)[Index];
	return &Record;
}


bool UInventorySubsystem::AddItem2(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ContainerId, AssetId is invalid or Quantity is less than or equal to 0"));
		return false;
	}

	FName ItemId = AssetId.PrimaryAssetName;
	TArray<FInventoryRecord>* ItemStack = GetMutableItemStack(ContainerId, ItemId);
	if (!ItemStack)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryStack is invalid"));
		return false;
	}

	FName RecordId = TEXT_EMPTY;
	const FInventoryRecord* Record = AddItemRecord(AssetId, Quantity, ItemStack, RecordId);
	if (Record)
	{
		OnItemAdded.Broadcast(ContainerId, AssetId, Record);
		return true;
	}

	return false;
}

bool UInventorySubsystem::AddItems2(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items)
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!ContainerId.IsValid() || !InventoryInterface)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ContainerId or InventoryInterface is invalid"));
		return false;
	}

	FName CurrentItemId = TEXT_EMPTY;
	TArray<FInventoryRecord>* CurrentItemStack = nullptr;

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId AssetId = Kv.Key;
		int Quantity = Kv.Value;

		if (InventoryPrimaryAsset::IsValid(AssetId) && Quantity > 0)
		{
			if (CurrentItemId != AssetId.PrimaryAssetName)
			{
				CurrentItemId = AssetId.PrimaryAssetName;
				CurrentItemStack = GetMutableItemStack(ContainerId, CurrentItemId, InventoryInterface);
			}

			FName RecordId = TEXT_EMPTY;
			const FInventoryRecord* Record = AddItemRecord(AssetId, Quantity, CurrentItemStack, RecordId);
			if (Record)
			{
				OnItemAdded.Broadcast(ContainerId, AssetId, Record);
			}
		}
	}

	return true;
}


bool UInventorySubsystem::RemoveItem2(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ContainerId, AssetId is invalid or Quantity is less than or equal to 0"));
		return false;
	}

	FName ItemId = AssetId.PrimaryAssetName;
	TArray<FInventoryRecord>* ItemStack = GetMutableItemStack(ContainerId, ItemId);
	if (!ItemStack)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryStack is invalid"));
		return false;
	}

	bool bRemoved = false;
	const FInventoryRecord* Record = RemoveItemRecord(AssetId, Quantity, ItemStack, bRemoved);
	if (Record && bRemoved)
	{
		OnItemRemoved.Broadcast(ContainerId, AssetId, Record);
		return true;
	}

	return false;
}

bool UInventorySubsystem::RemoveItems2(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items)
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!ContainerId.IsValid() || !InventoryInterface)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ContainerId, InventoryInterface is invalid"));
		return false;
	}

	FName CurrentItemId = TEXT_EMPTY;
	TArray<FInventoryRecord>* CurrentItemStack = nullptr;

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId AssetId = Kv.Key;
		int Quantity = Kv.Value;

		if (InventoryPrimaryAsset::IsValid(AssetId) && Quantity > 0)
		{
			if (CurrentItemId != AssetId.PrimaryAssetName)
			{
				CurrentItemId = AssetId.PrimaryAssetName;
				CurrentItemStack = GetMutableItemStack(ContainerId, CurrentItemId, InventoryInterface);
			}

			bool bRemoved = false;
			const FInventoryRecord* Record = RemoveItemRecord(AssetId, Quantity, CurrentItemStack, bRemoved);
			if (Record && bRemoved)
			{
				OnItemRemoved.Broadcast(ContainerId, AssetId, Record);
			}
		}
	}

	return true;
}

bool UInventorySubsystem::RemoveItemById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, int Quantity)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ContainerId, AssetId is invalid or Quantity is less than or equal to 0"));
		return false;
	}

	FName ItemId = AssetId.PrimaryAssetName;
	TArray<FInventoryRecord>* ItemStack = GetMutableItemStack(ContainerId, ItemId);
	if (!ItemStack)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryStack is invalid"));
		return false;
	}

	bool bRemoved = false;
	const FInventoryRecord* Record = RemoveItemRecordById(AssetId, RecordId, Quantity, ItemStack, bRemoved);
	if (Record && bRemoved)
	{
		OnItemRemoved.Broadcast(ContainerId, AssetId, Record);
		return true;
	}

	return false;
}


bool UInventorySubsystem::ContainsItem2(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity) const
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
	{
		return false;
	}

	FName ItemId = AssetId.PrimaryAssetName;
	const TArray<FInventoryRecord>* ItemStack = GetItemStack(ContainerId, ItemId);
	if (!ItemStack || !ItemStack->IsValidIndex(0))
	{
		return false;
	}

	const FInventoryRecord& Record = (*ItemStack)[0];

	return Record.ItemQuantity >= Quantity;
}

bool UInventorySubsystem::ContainsItems2(FName ContainerId, const TMap<FPrimaryAssetId, int>& Items) const
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!ContainerId.IsValid() || !InventoryInterface)
	{
		return false;
	}

	for (const TPair<FPrimaryAssetId, int>& Kv : Items)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int Quantity = Kv.Value;

		if (!InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0)
		{
			return false;
		}

		FName ItemId = AssetId.PrimaryAssetName;
		const TArray<FInventoryRecord>* ItemStack = GetItemStack(ContainerId, ItemId, InventoryInterface);
		if (!ItemStack || !ItemStack->IsValidIndex(0))
		{
			return false;
		}

		const FInventoryRecord& Record = (*ItemStack)[0];
		if (Record.ItemQuantity < Quantity)
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

	FName ItemId = AssetId.PrimaryAssetName;
	const TArray<FInventoryRecord>* ItemStack = GetItemStack(ContainerId, ItemId);
	if (!ItemStack || !ItemStack->IsValidIndex(0))
	{
		return false;
	}

	const FInventoryRecord* Record = ItemStack->FindByPredicate([RecordId](const FInventoryRecord& Record) { return Record.ItemId == RecordId; });
	if (!Record)
	{
		return false;
	}

	return Record->ItemQuantity >= Quantity;
}


bool UInventorySubsystem::UpdateItem2(FName ContainerId, const FPrimaryAssetId& AssetId, TFunctionRef<bool(FInventoryRecord*)> InCallback)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId))
	{
		return false;
	}

	FInventoryRecord* Record = GetMutableRecordByIndex(ContainerId, AssetId);
	if (!InCallback(Record))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Record update callback returned false"));
		return false;
	}

	// OnItemUpdated.Broadcast(ContainerId, ItemId, &Record);
	return true;
}

bool UInventorySubsystem::UpdateItemById(FName ContainerId, const FPrimaryAssetId& AssetId, FName RecordId, TFunctionRef<bool(FInventoryRecord*)> InCallback)
{
	if (!ContainerId.IsValid() || !InventoryPrimaryAsset::IsValid(AssetId))
	{
		return false;
	}

	FInventoryRecord* Record = GetMutableRecordById(ContainerId, AssetId, RecordId);
	if (!InCallback(Record))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Record update callback returned false"));
		return false;
	}

	OnItemUpdated.Broadcast(ContainerId, AssetId, Record);
	return true;
}

int UInventorySubsystem::GetItemCount(FName ContainerId, const FPrimaryAssetId& AssetId) const
{
	return GetItemCount(ContainerId, AssetId, InventoryProvider.Get());
}

int UInventorySubsystem::GetItemCount(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface) const
{
	FName ItemId = AssetId.PrimaryAssetName;
	const TArray<FInventoryRecord>* ItemStack = GetItemStack(ContainerId, ItemId, InventoryInterface);
	if (!ItemStack)
	{
		return 0;
	}

	return ItemStack->Num();
}

int UInventorySubsystem::GetItemQuantity(FName ContainerId, const FPrimaryAssetId& AssetId, int Index) const
{
	return GetItemQuantity(ContainerId, AssetId, InventoryProvider.Get(), Index);
}

int UInventorySubsystem::GetItemQuantity(FName ContainerId, const FPrimaryAssetId& AssetId, IInventoryProviderInterface* InventoryInterface, int Index) const
{
	FName ItemId = AssetId.PrimaryAssetName;
	const TArray<FInventoryRecord>* ItemStack = GetItemStack(ContainerId, ItemId, InventoryInterface);
	if (!ItemStack)
	{
		return 0;
	}

	if (!ItemStack->IsValidIndex(Index))
	{
		return 0;
	}

	return (*ItemStack)[Index].ItemQuantity;
}








void UInventorySubsystem::PreAddItem(FInventoryRecord& Record)
{

}

const FInventoryRecord* UInventorySubsystem::AddItemRecord(const FPrimaryAssetId& InAssetId, int InQuantity, TArray<FInventoryRecord>* InItemStack, FName& OutRecordId)
{
	if (!InventoryPrimaryAsset::IsValid(InAssetId) || InQuantity <= 0 || !InItemStack)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InAssetId, InItemStack is invalid or InQuantity less than or equal to 0"));

		OutRecordId = TEXT_EMPTY;
		return nullptr;
	}

	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(InAssetId, AssetData))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("AssetData not found for InAssetId"));

		OutRecordId = TEXT_EMPTY;
		return nullptr;
	}

	FName ItemId = InAssetId.PrimaryAssetName;
	int Quantity = InQuantity;
	FName RecordId = ItemId;

	bool bIsStackable = false;
	InventoryPrimaryAsset::GetItemIsStackable(AssetData, bIsStackable);
	if (bIsStackable)
	{
		if (InItemStack->IsValidIndex(0))
		{
			FInventoryRecord& Record = (*InItemStack)[0];
			Record.ItemQuantity += Quantity;

			PRINT_INFO(LogTemp, 1.0f, TEXT("Record updated"));

			OutRecordId = Record.ItemId;
			return &Record;
		}
	}
	else
	{
		Quantity = 1;
		RecordId = FName(FGuid::NewGuid().ToString());
	}

	EInventoryItemType ItemType;
	if (!InventoryPrimaryAsset::GetItemType(AssetData, ItemType))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item type not found"));

		OutRecordId = TEXT_EMPTY;
		return nullptr;
	}

	FInventoryRecord NewRecord;
	NewRecord.ItemId = RecordId;
	NewRecord.ItemType = ItemType;
	NewRecord.ItemQuantity = Quantity;

	PreAddItem(NewRecord);

	InItemStack->Add(NewRecord);

	PRINT_INFO(LogTemp, 1.0f, TEXT("New record added"));

	OutRecordId = RecordId;
	return &NewRecord;
}

const FInventoryRecord* UInventorySubsystem::RemoveItemRecord(const FPrimaryAssetId& InAssetId, int InQuantity, TArray<FInventoryRecord>* InItemStack, bool bOutRemoved)
{
	if (InventoryPrimaryAsset::IsValid(InAssetId) || InQuantity <= 0 || !InItemStack)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InAssetId, InItemStack is invalid or InQuantity less than or equal to 0"));

		bOutRemoved = false;
		return nullptr;
	}

	if (!InItemStack->IsValidIndex(0))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InItemStack is empty"));

		bOutRemoved = false;
		return nullptr;
	}

	FInventoryRecord& Record = (*InItemStack)[0];
	return RemoveItemRecord_Internal(InAssetId, &Record, InQuantity, InItemStack, bOutRemoved);
}

const FInventoryRecord* UInventorySubsystem::RemoveItemRecordById(const FPrimaryAssetId& InAssetId, FName InRecordId, int InQuantity, TArray<FInventoryRecord>* InItemStack, bool bOutRemoved)
{
	if (InRecordId.IsNone() || InQuantity <= 0 || !InItemStack)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InRecordId, InItemStack is invalid or InQuantity less than or equal to 0"));

		bOutRemoved = false;
		return nullptr;
	}

	FInventoryRecord* Record = InItemStack->FindByPredicate([InRecordId](const FInventoryRecord& Record) { return Record.ItemId == InRecordId; });

	return RemoveItemRecord_Internal(InAssetId, Record, InQuantity, InItemStack, bOutRemoved);
}

const FInventoryRecord* UInventorySubsystem::RemoveItemRecord_Internal(const FPrimaryAssetId& InAssetId, FInventoryRecord* InRecord, int InQuantity, TArray<FInventoryRecord>* InItemStack, bool bOutRemoved)
{
	if (!InRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Record is invalid"));

		bOutRemoved = false;
		return nullptr;
	}

	int FinalQuantity = InRecord->ItemQuantity - InQuantity;
	if (FinalQuantity > 0)
	{
		InRecord->ItemQuantity = FinalQuantity;
		PRINT_INFO(LogTemp, 1.0f, TEXT("Record quantity reduced"));

		bOutRemoved = true;
		return InRecord;
	}

	FPrimaryAssetId AssetId = InventoryPrimaryAsset::GetPrimaryAssetId(InAssetId.PrimaryAssetName);
	FAssetData AssetData;
	if (!AssetManager->GetPrimaryAssetData(AssetId, AssetData))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("AssetData not found for AssetId"));

		bOutRemoved = false;
		return nullptr;
	}

	bool bAllowEmptyData = false;
	InventoryPrimaryAsset::GetItemAllowEmptyData(AssetData, bAllowEmptyData);
	if (bAllowEmptyData)
	{
		InRecord->ItemQuantity = 0;
		PRINT_INFO(LogTemp, 1.0f, TEXT("Record quantity reduced to zero"));

		bOutRemoved = true;
		return InRecord;
	}

	InItemStack->Remove(*InRecord);
	PRINT_INFO(LogTemp, 1.0f, TEXT("Record removed"));

	bOutRemoved = true;
	return nullptr;
}





void UInventorySubsystem::HandleGlossaryItems2(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> InCallback) const
{
	IInventoryProviderInterface* InventoryInterface = InventoryProvider.Get();
	if (!IsValid(AssetManager) || !InventoryInterface)
	{
		LOG_ERROR(LogTemp, TEXT("AssetManager, InventoryInterface is invalid"));
		return;
	}

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
		FText ItemName = FText::GetEmpty();
		InventoryPrimaryAsset::GetItemName(AssetData, ItemName);

		FName ItemType = NAME_None;
		InventoryPrimaryAsset::GetItemType(AssetData, ItemType);

		FName ItemRarity = NAME_None;
		InventoryPrimaryAsset::GetItemRarity(AssetData, ItemRarity);

		bool bIsStackable = false;
		InventoryPrimaryAsset::GetItemIsStackable(AssetData, bIsStackable);

		if (!FilterRule.Match(ItemId, ItemType, ItemRarity))
		{
			continue;
		}

		int ItemQuantity = 0;
		if (bIsStackable)
		{
			ItemQuantity = GetItemQuantity(ContainerId, AssetId, InventoryInterface);
		}
		else
		{
			ItemQuantity = GetItemCount(ContainerId, AssetId, InventoryInterface);
		}

		SortedItems.Emplace(AssetId, ItemName, ItemQuantity);
	}

	HandleItemSorting(SortedItems, QueryRule);

	for (const FInventorySortEntry& Item : SortedItems)
	{
		InCallback(Item);
	}
}

void UInventorySubsystem::HandleInventoryItems2(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FInventorySortEntry&)> InCallback) const
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

	TArray<FInventorySortEntry> SortedItems;

	const TMap<FName, FInventoryStack>& Stacks = Container->ItemStacks;
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

		FText ItemName = FText::GetEmpty();
		bool bNameValid = InventoryPrimaryAsset::GetItemName(AssetData, ItemName);

		FName ItemType = TEXT_EMPTY;
		bool bTypeValid = InventoryPrimaryAsset::GetItemType(AssetData, ItemType);

		FName ItemRarity = TEXT_EMPTY;
		bool bRarityValid = InventoryPrimaryAsset::GetItemRarity(AssetData, ItemRarity);

		bool bIsStackable = false;
		bool bStackableValid = InventoryPrimaryAsset::GetItemIsStackable(AssetData, bIsStackable);

		if (!bNameValid || !bTypeValid || !bRarityValid || !bStackableValid)
		{
			continue;
		}

		if (!FilterRule.Match(ItemId, ItemType, ItemRarity))
		{
			continue;
		}

		int ItemQuantity = 0;
		if (bIsStackable)
		{
			ItemQuantity = GetItemQuantity(ContainerId, AssetId, InventoryInterface);
		}

		const TArray<FInventoryRecord>& Records = Stack->Records;
		for (const FInventoryRecord& Record : Records)
		{
			if (!FilterRule.Match(&Record))
			{
				continue;
			}

			if (!bIsStackable)
			{
				ItemQuantity = Record.ItemQuantity;
			}

			SortedItems.Emplace(AssetId, ItemName, ItemQuantity, &Record);
		}
	}

	HandleItemSorting(SortedItems, QueryRule);

	for (const FInventorySortEntry& Item : SortedItems)
	{
		InCallback(Item);
	}
}
