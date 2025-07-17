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



bool UInventorySubsystem::AddItem(UInventoryAsset* InventoryAsset, int Quantity)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr || !IsValid(InventoryAsset) || Quantity <= 0 )
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface, InventoryAsset is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();

	if (AddItemRecord_Internal(InventoryAsset->ItemId, InventoryAsset->ItemType, InventoryAsset->bIsStackable, Quantity, Records))
	{
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}

bool UInventorySubsystem::AddItems(const TMap<UInventoryAsset*, int32>& Items)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();
	for (const auto& Item : Items)
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

bool UInventorySubsystem::RemoveItems(const TMap<FName, int>& Items)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();
	for (const auto& Item : Items)
	{
		if (Item.Key.IsValid() && Item.Value > 0)
		{
			RemoveItemRecord_Internal(Item.Key, Item.Value, Records);
		}
	}

	OnInventoryUpdated.Broadcast();
	return true;
}

bool UInventorySubsystem::UpdateItem(FName ItemGuid, FInventoryRecord ItemRecord)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr || !ItemRecord.IsValid())
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface or ItemRecord is invalid"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();
	if (!Records.Contains(ItemGuid))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Record not found: %s"), *ItemGuid.ToString());
		return false;
	}

	Records.Add(ItemGuid, ItemRecord);

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

FInventoryRecord UInventorySubsystem::GetItemRecord(FName ItemGuid)
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
	else
	{
		return FInventoryRecord();
	}
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




void UInventorySubsystem::ForEachItem(TFunctionRef<void(const FName&, const FInventoryRecord&, UInventoryAsset*)> InCallback) const
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		LOG_ERROR(LogTemp, TEXT("InventoryInterfacePtr is invalid"));
		return;
	}

	const TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetInventoryRecords();
	for (const auto& Pair : Records)
	{
		const FInventoryRecord& Record = Pair.Value;
		UInventoryAsset* ItemAsset = GetItemAsset(Record.ItemId);

		if (Record.IsValid() && ItemAsset)
		{
			InCallback(Pair.Key, Record, ItemAsset);
		}
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

bool UInventorySubsystem::RemoveItemRecord_Internal(FName ItemId, int Quantity, TMap<FName, FInventoryRecord>& Records)
{
	if (Quantity <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Quantity less than or equal to 0"));
		return false;
	}

	FInventoryRecord* FoundRecord = Records.Find(ItemId);
	if (!FoundRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Record not found: %s"), *ItemId.ToString());
		return false;
	}

	if (FoundRecord->ItemQuantity > Quantity)
	{
		FoundRecord->ItemQuantity -= Quantity;
		PRINT_INFO(LogTemp, 1.0f, TEXT("Record quantity reduced: %s"), *ItemId.ToString());
	}
	else
	{
		Records.Remove(ItemId);
		PRINT_INFO(LogTemp, 1.0f, TEXT("Record removed: %s"), *ItemId.ToString());
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

