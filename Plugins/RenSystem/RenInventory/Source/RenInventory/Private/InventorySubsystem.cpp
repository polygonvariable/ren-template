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

	Containers.Add(ContainerId, FInventoryContainer());
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

			EInventoryItemType ItemType = EInventoryItemType::Default;
			bool bTypeValid = InventoryPrimaryAsset::GetItemType(AssetData, ItemType);

			EInventoryItemRarity ItemRarity = EInventoryItemRarity::Default;
			bool bRarityValid = InventoryPrimaryAsset::GetItemRarity(AssetData, ItemRarity);

			const FInventoryRecord* Record = GetItemRecord(ContainerId, ItemId);

			if (!FilterRule.Match(Record, ItemId, static_cast<uint8>(ItemType), static_cast<uint8>(ItemRarity)))
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

			EInventoryItemType ItemType = EInventoryItemType::Default;
			bool bTypeValid = InventoryPrimaryAsset::GetItemType(AssetData, ItemType);

			EInventoryItemRarity ItemRarity = EInventoryItemRarity::Default;
			bool bRarityValid = InventoryPrimaryAsset::GetItemRarity(AssetData, ItemRarity);

			const FInventoryRecord* Record = GetItemRecord(ContainerId, ItemId);

			if (!FilterRule.Match(Record, ItemId, static_cast<uint8>(ItemType), static_cast<uint8>(ItemRarity)))
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

			EInventoryItemType ItemType = EInventoryItemType::Default;
			bool bTypeValid = InventoryPrimaryAsset::GetItemType(AssetData, ItemType);

			EInventoryItemRarity ItemRarity = EInventoryItemRarity::Default;
			bool bRarityValid = InventoryPrimaryAsset::GetItemRarity(AssetData, ItemRarity);

			if (!bNameValid || !bTypeValid || !bRarityValid)
			{
				continue;
			}

			if (!FilterRule.Match(&Record, Record.ItemId, static_cast<uint8>(ItemType), static_cast<uint8>(ItemRarity)))
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

			EInventoryItemType ItemType = EInventoryItemType::Default;
			bool bTypeValid = InventoryPrimaryAsset::GetItemType(AssetData, ItemType);

			EInventoryItemRarity ItemRarity = EInventoryItemRarity::Default;
			bool bRarityValid = InventoryPrimaryAsset::GetItemRarity(AssetData, ItemRarity);

			if (!bTypeValid || !bRarityValid)
			{
				continue;
			}

			if (!FilterRule.Match(&Record, Record.ItemId, static_cast<uint8>(ItemType), static_cast<uint8>(ItemRarity)))
			{
				continue;
			}

			InCallback(AssetId, RecordId, &Record);
		}
	}
}

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
					A.Record->ItemQuantity < B.Record->ItemQuantity :
					A.Record->ItemQuantity > B.Record->ItemQuantity;
			}
		);
		break;
	case EInventorySortType::Level:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
				return (SortDirection == ESortDirection::Ascending) ?
					A.Record->EnhanceRecord.Level < B.Record->EnhanceRecord.Level :
					A.Record->EnhanceRecord.Level > B.Record->EnhanceRecord.Level;
			}
		);
		break;
	case EInventorySortType::Rank:
		SortedItems.Sort([SortDirection](const FInventorySortEntry& A, const FInventorySortEntry& B)
			{
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

