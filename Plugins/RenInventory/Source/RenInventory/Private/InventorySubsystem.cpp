// Fill out your copyright notice in the Description page of Project Settings.

//
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



bool UInventorySubsystem::InternalAddRecord(FName ItemId, EInventoryItemType ItemType, bool bIsStackable, int Quantity, TMap<FName, FInventoryRecord>& Records)
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

bool UInventorySubsystem::AddRecord(UInventoryAsset* InventoryAsset, int Quantity)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr || !IsValid(InventoryAsset) || Quantity <= 0 )
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface, InventoryAsset is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();

	return InternalAddRecord(InventoryAsset->AssetId, InventoryAsset->ItemType, InventoryAsset->bIsStackable, Quantity, Records);
}

bool UInventorySubsystem::AddRecords(const TMap<UInventoryAsset*, int32>& Items)
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
			InternalAddRecord(Item.Key->AssetId, Item.Key->ItemType, Item.Key->bIsStackable, Item.Value, Records);
		}
	}

	OnInventoryUpdated.Broadcast();

	return true;
}


bool UInventorySubsystem::InternalRemoveRecord(FName ItemId, int Quantity, TMap<FName, FInventoryRecord>& Records)
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

bool UInventorySubsystem::RemoveRecord(FName ItemGuid, int Quantity)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr || Quantity <= 0)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid or Quantity less than or equal to 0"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();
	
	return InternalRemoveRecord(ItemGuid, Quantity, Records);
}

bool UInventorySubsystem::RemoveRecords(const TMap<FName, int32>& Items)
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
			InternalRemoveRecord(Item.Key, Item.Value, Records);
		}
	}

	OnInventoryUpdated.Broadcast();

	return true;
}

bool UInventorySubsystem::UpdateRecord(FName ItemGuid, FInventoryRecord InventoryRecord)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr || !InventoryRecord.IsValid())
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface or InventoryRecord is invalid"));
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetMutableInventoryRecords();
	if (!Records.Contains(ItemGuid))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Record not found: %s"), *ItemGuid.ToString());
		return false;
	}

	Records.Add(ItemGuid, InventoryRecord);

	OnInventoryUpdated.Broadcast();

	PRINT_INFO(LogTemp, 1.0f, TEXT("Record updated: %s"), *ItemGuid.ToString());

	return true;
}

bool UInventorySubsystem::HasRecord(FName ItemGuid)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid"));
		return false;
	}

	return InventoryInterfacePtr->GetInventoryRecords().Contains(ItemGuid);
}

bool UInventorySubsystem::GetRecord(FName ItemGuid, FInventoryRecord& OutInventoryRecord)
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid"));
		OutInventoryRecord = FInventoryRecord();
		return false;
	}

	const TMap<FName, FInventoryRecord>& Records = InventoryInterfacePtr->GetInventoryRecords();

	if (const FInventoryRecord* FoundRecord = Records.Find(ItemGuid))
	{
		OutInventoryRecord = *FoundRecord;
		return OutInventoryRecord.IsValid();
	}
	else
	{
		OutInventoryRecord = FInventoryRecord();
		return false;
	}
}

TMap<FName, FInventoryRecord> UInventorySubsystem::GetRecords()
{
	IInventoryProviderInterface* InventoryInterfacePtr = InventoryInterface.Get();
	if (!InventoryInterfacePtr)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryInterface is invalid"));
		return TMap<FName, FInventoryRecord>();
	}

	return InventoryInterfacePtr->GetMutableInventoryRecords();
}

UInventoryAsset* UInventorySubsystem::GetRecordAsset(FName InventoryAssetId)
{
	if (!IsValid(InventoryAssetMap))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventoryAssetMap is invalid"));
		return nullptr;
	}
	
	return InventoryAssetMap->GetAssetByName<UInventoryAsset>(InventoryAssetId);
}





void UInventorySubsystem::HandleStorageLoaded()
{
	FLatentDelegates::OnStorageLoaded.RemoveAll(this);

	IStorageProviderInterface* StorageInterface = SubsystemUtils::GetSubsystemInterface<UGameInstance, UGameInstanceSubsystem, IStorageProviderInterface>(GetGameInstance());
	if (!StorageInterface)
	{
		LOG_ERROR(LogTemp, TEXT("StorageInterface is invalid"));
		return;
	}

	USaveGame* SaveGame = StorageInterface->GetLocalStorage();
	if (!IsValid(SaveGame) || !SaveGame->Implements<UInventoryProviderInterface>())
	{
		LOG_ERROR(LogTemp, TEXT("SaveGame is invalid or does not implement IInventoryProviderInterface"));
		return;
	}

	IInventoryProviderInterface* InventoryInterfacePtr = Cast<IInventoryProviderInterface>(SaveGame);
	if (!InventoryInterfacePtr)
	{
		LOG_ERROR(LogTemp, TEXT("InventoryInterface cast failed"));
		return;
	}

	InventoryInterface = TWeakInterfacePtr<IInventoryProviderInterface>(InventoryInterfacePtr);
}




bool UInventorySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, "InventorySubsystem Initialized");

	if (!FLatentDelegates::OnStorageLoaded.IsBoundToObject(this))
	{
		FLatentDelegates::OnStorageLoaded.AddUObject(this, &UInventorySubsystem::HandleStorageLoaded);
	}

	const UGameMetadataSettings* GameMetadata = GetDefault<UGameMetadataSettings>();
	if (!IsValid(GameMetadata) || GameMetadata->InventoryAssetMap.IsNull())
	{
		LOG_ERROR(LogTemp, "GameMetadata or InventoryAssetMap is invalid");
		return;
	}
	
	UPrimaryDataAsset* LoadedAsset = Cast<UPrimaryDataAsset>(GameMetadata->InventoryAssetMap.LoadSynchronous());
	if (!IsValid(LoadedAsset))
	{
		LOG_ERROR(LogTemp, TEXT("Failed to load or cast InventoryAssetMap"));
		return;
	}

	UPrimaryAssetMap* AssetMap = Cast<UPrimaryAssetMap>(LoadedAsset);
	if (!IsValid(AssetMap))
	{
		LOG_ERROR(LogTemp, TEXT("InventoryAssetMap is not of type UPrimaryAssetMap"));
		return;
	}

	InventoryAssetMap = AssetMap;
}

void UInventorySubsystem::Deinitialize()
{
	InventoryAssetMap = nullptr;
	InventoryInterface.Reset();

	FLatentDelegates::OnStorageLoaded.RemoveAll(this);

	LOG_WARNING(LogTemp, "InventorySubsystem Deinitialized");
	Super::Deinitialize();
}

