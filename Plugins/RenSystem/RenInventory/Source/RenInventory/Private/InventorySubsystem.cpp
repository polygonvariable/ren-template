// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InventorySubsystem.h"

// Engine Headers
// #include "Engine/AssetManager.h"

// Project Headers
#include "RenAsset/Public/PrimaryAssetMap.h"

#include "RCoreDelegate/Public/LatentDelegates.h"
#include "RCoreSettings/Public/GameMetadataSettings.h"

#include "RCoreStorage/Public/StorageProviderInterface.h"

#include "RCoreInventory/Public/InventoryAsset.h"
#include "RCoreInventory/Public/InventoryItemType.h"
#include "RCoreInventory/Public/InventoryProviderInterface.h"
#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/SubsystemUtils.h"
#include "RCoreLibrary/Public/LogMacro.h"




bool UInventorySubsystem::BP_AddItem(FName ContainerId, UInventoryAsset* ItemAsset, int Quantity)
{
	return AddItem(ContainerId, ItemAsset, Quantity);
}

bool UInventorySubsystem::BP_AddItems(FName ContainerId, const TMap<UInventoryAsset*, int>& ItemQuantities)
{
	return AddItems(ContainerId, ItemQuantities);
}



bool UInventorySubsystem::AddItem(FName ContainerId, UInventoryAsset* ItemAsset, int Quantity)
{
	if (!ItemAsset)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ItemAsset is invalid"));
		return false;
	}

	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container: %s"), *ContainerId.ToString());
		return false;
	}

	if (AddItemRecord_Internal(ContainerId, ItemAsset->ItemId, ItemAsset->ItemType, ItemAsset->bIsStackable, Quantity, Records))
	{
		return true;
	}

	return false;
}

bool UInventorySubsystem::AddItems(FName ContainerId, const TMap<UInventoryAsset*, int>& ItemQuantities)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container: %s"), *ContainerId.ToString());
		return false;
	}

	for (const auto& Item : ItemQuantities)
	{
		UInventoryAsset* Asset = Item.Key;
		int Quantity = Item.Value;

		if (Asset && Quantity > 0)
		{
			AddItemRecord_Internal(ContainerId, Asset->ItemId, Asset->ItemType, Asset->bIsStackable, Quantity, Records);
		}
	}

	return true;
}

bool UInventorySubsystem::RemoveItem(FName ContainerId, FName ItemGuid, int Quantity)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container: %s"), *ContainerId.ToString());
		return false;
	}

	if (RemoveItemRecord_Internal(ItemGuid, Quantity, Records))
	{
		return true;
	}

	return false;
}

bool UInventorySubsystem::RemoveItems(FName ContainerId, const TMap<FName, int>& ItemQuantities)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container: %s"), *ContainerId.ToString());
		return false;
	}

	for (const auto& Item : ItemQuantities)
	{
		if (Item.Key.IsValid() && Item.Value > 0)
		{
			RemoveItemRecord_Internal(Item.Key, Item.Value, Records);
		}
	}

	return true;
}

bool UInventorySubsystem::DeleteItem(FName ContainerId, FName ItemGuid)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container: %s"), *ContainerId.ToString());
		return false;
	}

	if (Records->Remove(ItemGuid) <= 0)
	{
		PRINT_WARNING(LogTemp, 1.0f, TEXT("No item deleted"));
		return false;
	}

	PRINT_INFO(LogTemp, 1.0f, TEXT("Item deleted"));

	return true;
}

bool UInventorySubsystem::DeleteItems(FName ContainerId, const TSet<FName>& ItemGuids)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container: %s"), *ContainerId.ToString());
		return false;
	}

	int Count = 0;
	for (const auto& Guid : ItemGuids)
	{
		Count += Records->Remove(Guid);
	}

	PRINT_INFO(LogTemp, 1.0f, TEXT("Items deleted: %d"), Count);

	return true;
}

bool UInventorySubsystem::ClearItems(FName ContainerId)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container: %s"), *ContainerId.ToString());
		return false;
	}

	Records->Empty();

	PRINT_INFO(LogTemp, 1.0f, TEXT("Items cleared"));

	return true;
}

bool UInventorySubsystem::ReplaceItem(FName ContainerId, FName ItemGuid, FInventoryRecord Record)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container: %s"), *ContainerId.ToString());
		return false;
	}

	Records->Add(ItemGuid, Record);

	PRINT_INFO(LogTemp, 1.0f, TEXT("Record replaced: %s"), *ItemGuid.ToString());

	return true;
}

int UInventorySubsystem::CountItem(FName ContainerId, FName ItemId) const
{
	const TMap<FName, FInventoryRecord>* Records = GetRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container: %s"), *ContainerId.ToString());
		return 0;
	}

	int Count = 0;
	for (const auto& Record : *Records)
	{
		if (Record.Value.ItemId == ItemId)
		{
			Count++;
		}
	}

	return Count;
}

bool UInventorySubsystem::UpdateItem(FName ContainerId, FName ItemGuid, TFunctionRef<bool(FInventoryRecord*)> InCallback)
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Records not found in container: %s"), *ContainerId.ToString());
		return false;
	}

	FInventoryRecord* Record = Records->Find(ItemGuid);
	if (!Record)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Record not found: %s"), *ItemGuid.ToString());
		return false;
	}

	if (!InCallback(Record))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Record update callback returned false: %s"), *ItemGuid.ToString());
		return false;
	}

	OnItemUpdated.Broadcast(ContainerId, ItemGuid, Record);

	return true;
}




bool UInventorySubsystem::ContainsItem(FName ContainerId, FName ItemGuid, int Quantity) const
{
	const TMap<FName, FInventoryRecord>* Records = GetRecords(ContainerId);
	if (!Records)
	{
		return false;
	}

	const FInventoryRecord* Record = Records->Find(ItemGuid);
	if (!Record)
	{
		return false;
	}

	return Record->ItemQuantity >= Quantity;
}

bool UInventorySubsystem::ContainsItems(FName ContainerId, const TMap<FName, int>& ItemQuantities) const
{
	const TMap<FName, FInventoryRecord>* Records = GetRecords(ContainerId);
	if (!Records)
	{
		return false;
	}

	for (const auto& Item : ItemQuantities)
	{
		const FInventoryRecord* Record = Records->Find(Item.Key);
		if (!Record || Record->ItemQuantity < Item.Value)
		{
			return false;
		}
	}

	return true;
}





bool UInventorySubsystem::CreateContainer(FName ContainerId)
{
	IInventoryProviderInterface* InventoryProvider = InventoryInterface.Get();
	if (!InventoryProvider)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid"));
		return false;
	}

	TMap<FName, FInventoryContainer>& Containers = InventoryProvider->GetMutableInventoryContainer();
	if (Containers.Contains(ContainerId))
	{
		PRINT_WARNING(LogTemp, 1.0f, TEXT("Container already exists: %s"), *ContainerId.ToString());
		return false;
	}

	Containers.Add(ContainerId, FInventoryContainer());
	OnContainerAdded.Broadcast(ContainerId);

	PRINT_INFO(LogTemp, 1.0f, TEXT("Container created: %s"), *ContainerId.ToString());
	return true;
}

bool UInventorySubsystem::RemoveContainer(FName ContainerId)
{
	IInventoryProviderInterface* InventoryProvider = InventoryInterface.Get();
	if (!InventoryProvider)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid"));
		return false;
	}

	TMap<FName, FInventoryContainer>& Containers = InventoryProvider->GetMutableInventoryContainer();
	if (Containers.Remove(ContainerId) <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Container not found: %s"), *ContainerId.ToString());
		return false;
	}

	OnContainerRemoved.Broadcast(ContainerId);

	PRINT_INFO(LogTemp, 1.0f, TEXT("Container removed: %s"), *ContainerId.ToString());
	return true;
}



TMap<FName, FInventoryRecord>* UInventorySubsystem::GetMutableRecords(FName ContainerId) const
{
	IInventoryProviderInterface* InventoryProvider = InventoryInterface.Get();
	if (!InventoryProvider)
	{
		return nullptr;
	}

	TMap<FName, FInventoryContainer>& Containers = InventoryProvider->GetMutableInventoryContainer();
	FInventoryContainer* Records = Containers.Find(ContainerId);

	if (!Records)
	{
		return nullptr;
	}

	return &Records->Items;
}

const TMap<FName, FInventoryRecord>* UInventorySubsystem::GetRecords(FName ContainerId) const
{
	IInventoryProviderInterface* InventoryProvider = InventoryInterface.Get();
	if (!InventoryProvider)
	{
		return nullptr;
	}

	const TMap<FName, FInventoryContainer>& Containers = InventoryProvider->GetInventoryContainer();
	const FInventoryContainer* Records = Containers.Find(ContainerId);

	if (!Records)
	{
		return nullptr;
	}

	return &Records->Items;
}




FInventoryRecord* UInventorySubsystem::GetMutableItemRecord(FName ContainerId, FName ItemGuid) const
{
	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		return nullptr;
	}
	return Records->Find(ItemGuid);
}

const FInventoryRecord* UInventorySubsystem::GetItemRecord(FName ContainerId, FName ItemGuid) const
{
	const TMap<FName, FInventoryRecord>* Records = GetRecords(ContainerId);
	if (!Records)
	{
		return nullptr;
	}
	return Records->Find(ItemGuid);
}

UInventoryAsset* UInventorySubsystem::GetItemAsset(FName ItemId) const
{
	if (!InventoryAssetMap)
	{
		return nullptr;
	}

	return InventoryAssetMap->GetAssetById<UInventoryAsset>(ItemId);
}


void UInventorySubsystem::QueryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FName&, const FInventoryRecord*, UInventoryAsset*)> InCallback) const
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

void UInventorySubsystem::HandleGlossaryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FName&, const FInventoryRecord*, UInventoryAsset*)> InCallback) const
{
	const TMap<FName, TObjectPtr<UPrimaryDataAsset>>& Glossary = InventoryAssetMap->AssetMapping;
	const FName& ContainerId = QueryRule.ContainerId;

	if (QueryRule.SortType != EInventorySortType::None)
	{
		TArray<FInventorySortEntry> SortedItems;
		SortedItems.Reserve(Glossary.Num());

		for (const auto& Item : Glossary)
		{
			UInventoryAsset* Asset = Cast<UInventoryAsset>(Item.Value);
			if (!Asset)
			{
				continue;
			}

			const FInventoryRecord* Record = GetItemRecord(ContainerId, Item.Key);
			if (!FilterRule.Match(Record, Asset->ItemId, Asset->ItemType, Asset->ItemRarity))
			{
				continue;
			}

			SortedItems.Emplace(Item.Key, Record, Asset);
		}

		HandleItemSorting(SortedItems, QueryRule);

		for (const auto& Item : SortedItems)
		{
			InCallback(Item.Guid, Item.Record, Item.Asset);
		}
	}
	else
	{
		for (const auto& Item : Glossary)
		{
			UInventoryAsset* Asset = Cast<UInventoryAsset>(Item.Value);
			if (!Asset)
			{
				continue;
			}

			const FInventoryRecord* Record = GetItemRecord(ContainerId, Item.Key);
			if (!FilterRule.Match(Record, Asset->ItemId, Asset->ItemType, Asset->ItemRarity))
			{
				continue;
			}

			InCallback(Item.Key, Record, Asset);
		}
	}

}

void UInventorySubsystem::HandleInventoryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FName&, const FInventoryRecord*, UInventoryAsset*)> InCallback) const
{
	const FName& ContainerId = QueryRule.ContainerId;

	TMap<FName, FInventoryRecord>* Records = GetMutableRecords(ContainerId);
	if (!Records)
	{
		LOG_ERROR(LogTemp, TEXT("Records not found in container: %s"), *ContainerId.ToString());
		return;
	}

	if (QueryRule.SortType != EInventorySortType::None)
	{
		TArray<FInventorySortEntry> SortedItems;
		SortedItems.Reserve(Records->Num());

		for (auto It = Records->CreateIterator(); It; ++It)
		{
			const FInventoryRecord& Record = It.Value();
			UInventoryAsset* Asset = GetItemAsset(Record.ItemId);

			if (!Asset || !Record.IsValid())
			{
				continue;
			}

			if (!FilterRule.Match(&Record, Asset->ItemId, Asset->ItemType, Asset->ItemRarity))
			{
				continue;
			}

			SortedItems.Emplace(It.Key(), &Record, Asset);
		}

		HandleItemSorting(SortedItems, QueryRule);

		for (const auto& Item : SortedItems)
		{
			InCallback(Item.Guid, Item.Record, Item.Asset);
		}
	}
	else
	{
		for (auto It = Records->CreateIterator(); It; ++It)
		{
			const FInventoryRecord& Record = It.Value();
			UInventoryAsset* Asset = GetItemAsset(Record.ItemId);

			if (!Asset || !Record.IsValid())
			{
				continue;
			}

			if (!FilterRule.Match(&Record, Asset->ItemId, Asset->ItemType, Asset->ItemRarity))
			{
				continue;
			}

			InCallback(It.Key(), &Record, Asset);
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
					A.Asset->ItemName.ToString() < B.Asset->ItemName.ToString() :
					A.Asset->ItemName.ToString() > B.Asset->ItemName.ToString();
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




bool UInventorySubsystem::AddItemRecord_Internal(FName ContainerId, FName ItemId, EInventoryItemType ItemType, bool bIsStackable, int Quantity, TMap<FName, FInventoryRecord>* Records)
{
	if (ItemId.IsNone() || Quantity <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ItemId is invalid or Quantity less than or equal to 0"));
		return false;
	}

	if (bIsStackable)
	{
		if (FInventoryRecord* Record = Records->Find(ItemId))
		{
			Record->ItemQuantity += Quantity;
			OnItemAdded.Broadcast(ContainerId, ItemId, Record);

			PRINT_INFO(LogTemp, 1.0f, TEXT("Stackable record %s updated"), *ItemId.ToString());
		}
		else
		{
			FInventoryRecord& NewRecord = Records->Add(ItemId, FInventoryRecord(
				{
					.ItemId = ItemId,
					.ItemType = ItemType,
					.ItemQuantity = Quantity
				}
			));
			OnItemAdded.Broadcast(ContainerId, ItemId, &NewRecord);

			PRINT_INFO(LogTemp, 1.0f, TEXT("Stackable record %s added"), *ItemId.ToString());
		}
	}
	else
	{
		FName Guid = FName(FGuid::NewGuid().ToString());
		FInventoryRecord& NewRecord = Records->Add(Guid, FInventoryRecord(
			{
				.ItemId = ItemId,
				.ItemType = ItemType,
				.ItemQuantity = Quantity
			}
		));
		OnItemAdded.Broadcast(ContainerId, Guid, &NewRecord);

		PRINT_INFO(LogTemp, 1.0f, TEXT("Non-stackable record %s added with guid: %s"), *ItemId.ToString(), *Guid.ToString());
	}

	return true;
}

bool UInventorySubsystem::RemoveItemRecord_Internal(FName ItemGuid, int Quantity, TMap<FName, FInventoryRecord>* Records)
{
	if (Quantity <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Quantity less than or equal to 0"));
		return false;
	}

	FInventoryRecord* Record = Records->Find(ItemGuid);
	if (!Record)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Record not found: %s"), *ItemGuid.ToString());
		return false;
	}

	int FinalQuantity = Record->ItemQuantity - Quantity;
	if (FinalQuantity > 0)
	{
		Record->ItemQuantity = FinalQuantity;
		PRINT_INFO(LogTemp, 1.0f, TEXT("Record quantity reduced: %s"), *ItemGuid.ToString());
	}
	else
	{
		bool bAllowEmptyData = false;
		UInventoryAsset* Asset = GetItemAsset(ItemGuid);

		if (Asset)
		{
			bAllowEmptyData = Asset->bAllowEmptyData;
		}

		if (bAllowEmptyData)
		{
			Record->ItemQuantity = 0;
			PRINT_INFO(LogTemp, 1.0f, TEXT("Record quantity reduced to zero: %s"), *ItemGuid.ToString());
		}
		else
		{
			Records->Remove(ItemGuid);
			PRINT_INFO(LogTemp, 1.0f, TEXT("Record removed: %s"), *ItemGuid.ToString());
		}
	}

	return true;
}



void UInventorySubsystem::HandleStorageLoaded()
{
	FLatentDelegates::OnStorageLoaded.RemoveAll(this);
	LOG_INFO(LogTemp, TEXT("InventorySubsystem storage load started"));

	IStorageProviderInterface* StorageInterface = SubsystemUtils::GetSubsystemInterface<UGameInstance, UGameInstanceSubsystem, IStorageProviderInterface>(GetGameInstance());
	if (!StorageInterface)
	{
		LOG_ERROR(LogTemp, TEXT("StorageInterface is invalid"));
		return;
	}

	UObject* Storage = StorageInterface->GetLocalStorage();
	if (!IsValid(Storage) || !Storage->Implements<UInventoryProviderInterface>())
	{
		LOG_ERROR(LogTemp, TEXT("Storage is invalid or does not implement IInventoryProviderInterface"));
		return;
	}

	IInventoryProviderInterface* InventoryProvider = Cast<IInventoryProviderInterface>(Storage);
	if (!InventoryProvider)
	{
		LOG_ERROR(LogTemp, TEXT("InventoryInterface is invalid"));
		return;
	}

	InventoryInterface = TWeakInterfacePtr<IInventoryProviderInterface>(InventoryProvider);
	LOG_INFO(LogTemp, TEXT("InventorySubsystem storage loaded"));
}

bool UInventorySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("InventorySubsystem Initialized"));

	if (!FLatentDelegates::OnStorageLoaded.IsBoundToObject(this))
	{
		FLatentDelegates::OnStorageLoaded.AddUObject(this, &UInventorySubsystem::HandleStorageLoaded);
	}

	const UGameMetadataSettings* GameMetadata = GetDefault<UGameMetadataSettings>();
	if (!IsValid(GameMetadata))
	{
		LOG_ERROR(LogTemp, TEXT("GameMetadata or InventoryAssetMap is invalid"));
		return;
	}

	if (GameMetadata->InventoryAssetMap.IsNull())
	{
		LOG_ERROR(LogTemp, TEXT("InventoryAssetMap is invalid"));
		return;
	}
	
	UPrimaryDataAsset* LoadedAsset = Cast<UPrimaryDataAsset>(GameMetadata->InventoryAssetMap.LoadSynchronous());
	if (!IsValid(LoadedAsset))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to load InventoryAssetMap"));
		return;
	}

	UPrimaryAssetMap* AssetMap = Cast<UPrimaryAssetMap>(LoadedAsset);
	if (!IsValid(AssetMap))
	{
		LOG_ERROR(LogTemp, TEXT("InventoryAssetMap cast failed"));
		return;
	}

	InventoryAssetMap = AssetMap;
}

void UInventorySubsystem::Deinitialize()
{
	InventoryAssetMap = nullptr;
	InventoryInterface.Reset();

	FLatentDelegates::OnStorageLoaded.RemoveAll(this);

	LOG_WARNING(LogTemp, TEXT("InventorySubsystem Deinitialized"));
	Super::Deinitialize();
}

