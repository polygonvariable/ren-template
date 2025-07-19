// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InventorySubsystem.h"

// Project Headers
#include "RenAsset/Public/Inventory/InventoryAsset.h"
#include "RenAsset/Public/PrimaryAssetMap.h"

#include "RenCore/Public/Delegate/LatentDelegates.h"
#include "RenCore/Public/Developer/GameMetadataSettings.h"
#include "RenCore/Public/Interface/InventoryProviderInterface.h"
#include "RenCore/Public/Interface/StorageProviderInterface.h"
#include "RenCore/Public/Inventory/InventoryItemType.h"
#include "RenCore/Public/Library/MiscLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"
#include "RenCore/Public/Record/InventoryRecord.h"



bool UInventorySubsystem::AddItem(UInventoryAsset* ItemAsset, int Quantity)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr || !IsValid(ItemAsset) || Quantity <= 0 )
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface, ItemAsset is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();

	if (AddItemRecord_Internal(ItemAsset->ItemId, ItemAsset->ItemType, ItemAsset->bIsStackable, Quantity, Records))
	{
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}

bool UInventorySubsystem::AddItems(const TMap<UInventoryAsset*, int32>& ItemQuantities)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();
	for (const auto& Item : ItemQuantities)
	{
		if (IsValid(Item.Key) && Item.Value > 0)
		{
			AddItemRecord_Internal(Item.Key->ItemId, Item.Key->ItemType, Item.Key->bIsStackable, Item.Value, Records);
		}
	}

	OnInventoryUpdated.Broadcast();
	return true;
}

bool UInventorySubsystem::RemoveItem(FName ItemGuid, int Quantity)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr || Quantity <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();
	
	if (RemoveItemRecord_Internal(ItemGuid, Quantity, Records))
	{
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}

bool UInventorySubsystem::RemoveItems(const TMap<FName, int>& ItemQuantities)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();
	for (const auto& Item : ItemQuantities)
	{
		if (Item.Key.IsValid() && Item.Value > 0)
		{
			RemoveItemRecord_Internal(Item.Key, Item.Value, Records);
		}
	}

	OnInventoryUpdated.Broadcast();
	return true;
}

bool UInventorySubsystem::DestroyItem(FName ItemGuid)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();
	if (Records.Remove(ItemGuid) <= 0)
	{
		PRINT_WARNING(LogTemp, 1.0f, TEXT("No item removed"));
		return false;
	}

	OnInventoryUpdated.Broadcast();
	return true;
}

bool UInventorySubsystem::DestroyItems(const TSet<FName>& ItemGuids)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();
	for (const auto& Guid : ItemGuids)
	{
		Records.Remove(Guid);
	}

	OnInventoryUpdated.Broadcast();
	return true;
}

bool UInventorySubsystem::ClearItems()
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();
	Records.Empty();

	OnInventoryUpdated.Broadcast();
	return true;
}

bool UInventorySubsystem::UpdateItem(FName ItemGuid, FInventoryRecord Record)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr || !Record.IsValid())
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface or Record is invalid"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();
	if (!Records.Contains(ItemGuid))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Record not found: %s"), *ItemGuid.ToString());
		return false;
	}

	Records.Add(ItemGuid, Record);

	PRINT_INFO(LogTemp, 1.0f, TEXT("Record updated: %s"), *ItemGuid.ToString());

	OnInventoryUpdated.Broadcast();
	return true;
}

bool UInventorySubsystem::ContainsItem(FName ItemGuid)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		return false;
	}
	return InventoryInterfacePtr->GetInventoryRecords().Contains(ItemGuid);
}

FInventoryRecord UInventorySubsystem::GetItemRecord(FName ItemGuid) const
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid"));
		return FInventoryRecord();
	}

	const TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetInventoryRecords();

	if (const FInventoryRecord* FoundRecord = Records.Find(ItemGuid))
	{
		return *FoundRecord;
	}
	return FInventoryRecord();
}

TMap<FName, FInventoryRecord> UInventorySubsystem::GetAllItemRecords()
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid"));
		return TMap<FName, FInventoryRecord>();
	}

	return InventoryInterfacePtr->GetMutableInventoryRecords();
}

UInventoryAsset* UInventorySubsystem::GetItemAsset(const FName& ItemId) const
{
	if (!InventoryAssetMap)
	{
		LOG_ERROR(LogTemp, TEXT("InventoryAssetMap is invalid"));
		return nullptr;
	}
	
	return InventoryAssetMap->GetAssetByName<UInventoryAsset>(ItemId);
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
	TMap<FName, TObjectPtr<UPrimaryDataAsset>> Glossary = InventoryAssetMap->AssetMapping;

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

			const FInventoryRecord Record = GetItemRecord(Item.Key);
			if (!FilterRule.Matches(Record, Asset->ItemId, Asset->ItemType, Asset->ItemRarity))
			{
				continue;
			}

			SortedItems.Emplace(Item.Key, &Record, Asset);
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

			const FInventoryRecord Record = GetItemRecord(Item.Key);
			if (!FilterRule.Matches(Record, Asset->ItemId, Asset->ItemType, Asset->ItemRarity))
			{
				continue;
			}

			InCallback(Item.Key, &Record, Asset);
		}
	}

}

void UInventorySubsystem::HandleInventoryItems(const FInventoryFilterRule& FilterRule, const FInventoryQueryRule& QueryRule, TFunctionRef<void(const FName&, const FInventoryRecord*, UInventoryAsset*)> InCallback) const
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		LOG_ERROR(LogTemp, TEXT("InventoryInterfacePtr is invalid"));
		return;
	}

	const TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetInventoryRecords();
	if (QueryRule.SortType != EInventorySortType::None)
	{
		TArray<FInventorySortEntry> SortedItems;
		SortedItems.Reserve(Records.Num());

		for (const auto& Item : Records)
		{
			const FInventoryRecord& Record = Item.Value;
			UInventoryAsset* Asset = GetItemAsset(Record.ItemId);

			if (!Asset || !Record.IsValid())
			{
				continue;
			}

			if (!FilterRule.Matches(Record, Asset->ItemId, Asset->ItemType, Asset->ItemRarity))
			{
				continue;
			}

			SortedItems.Emplace(Item.Key, &Record, Asset);
		}

		HandleItemSorting(SortedItems, QueryRule);

		for (const auto& Item : SortedItems)
		{
			InCallback(Item.Guid, Item.Record, Item.Asset);
		}
	}
	else
	{
		for (const auto& Item : Records)
		{
			const FInventoryRecord& Record = Item.Value;
			UInventoryAsset* Asset = GetItemAsset(Record.ItemId);

			if (!Asset || !Record.IsValid())
			{
				continue;
			}

			if (!FilterRule.Matches(Record, Asset->ItemId, Asset->ItemType, Asset->ItemRarity))
			{
				continue;
			}

			InCallback(Item.Key, &Record, Asset);
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




bool UInventorySubsystem::AddItemRecord_Internal(FName ItemId, EInventoryItemType ItemType, bool bIsStackable, int Quantity, TMap<FName, FInventoryRecord>& Records)
{
	if (ItemId.IsNone() || Quantity <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("ItemId is invalid or Quantity less than or equal to 0"));
		return false;
	}

	if (bIsStackable)
	{
		if (FInventoryRecord* FoundRecord = Records.Find(ItemId))
		{
			FoundRecord->ItemQuantity += Quantity;

			PRINT_INFO(LogTemp, 1.0f, TEXT("Stackable record %s updated"), *ItemId.ToString());
		}
		else
		{
			Records.Add(ItemId, FInventoryRecord(
				{
					.ItemId = ItemId,
					.ItemType = ItemType,
					.ItemQuantity = Quantity
				}
			));

			PRINT_INFO(LogTemp, 1.0f, TEXT("Stackable record %s added"), *ItemId.ToString());
		}
	}
	else
	{
		FName Guid = FName(FGuid::NewGuid().ToString());
		Records.Add(Guid, FInventoryRecord(
			{
				.ItemId = ItemId,
				.ItemType = ItemType,
				.ItemQuantity = Quantity
			}
		));

		PRINT_INFO(LogTemp, 1.0f, TEXT("Non-stackable record %s added with guid: %s"), *ItemId.ToString(), *Guid.ToString());
	}

	return true;
}

bool UInventorySubsystem::RemoveItemRecord_Internal(FName ItemGuid, int Quantity, TMap<FName, FInventoryRecord>& Records)
{
	if (Quantity <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Quantity less than or equal to 0"));
		return false;
	}

	FInventoryRecord* FoundRecord = Records.Find(ItemGuid);
	if (!FoundRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Record not found: %s"), *ItemGuid.ToString());
		return false;
	}

	if (FoundRecord->ItemQuantity > Quantity)
	{
		FoundRecord->ItemQuantity -= Quantity;
		PRINT_INFO(LogTemp, 1.0f, TEXT("Record quantity reduced: %s"), *ItemGuid.ToString());
	}
	else
	{
		Records.Remove(ItemGuid);
		PRINT_INFO(LogTemp, 1.0f, TEXT("Record removed: %s"), *ItemGuid.ToString());
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

	IInventoryProviderInterface* InventoryInterfacePtr = Cast<IInventoryProviderInterface>(Storage);
	if (!InventoryInterfacePtr)
	{
		LOG_ERROR(LogTemp, TEXT("InventoryInterface is invalid"));
		return;
	}

	InventoryInterface = TWeakInterfacePtr<IInventoryProviderInterface>(InventoryInterfacePtr);
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

