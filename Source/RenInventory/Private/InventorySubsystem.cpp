// Fill out your copyright notice in the Description page of Project Settings.

//
#include "InventorySubsystem.h"

// Project Headers
#include "RenAsset/Public/Inventory/InventoryAsset.h"

#include "RenCore/Public/Developer/GameMetadataSettings.h"

#include "RenCore/Public/Inventory/InventoryItemType.h"
#include "RenCore/Public/Macro/LogMacro.h"
#include "RenCore/Public/Record/InventoryRecord.h"

#include "RenStorage/Public/Storage.h"
#include "RenStorage/Public/StorageSubsystem.h"



bool UInventorySubsystem::AddRecord(UInventoryAsset* InventoryAsset, const int Quantity)
{
	if (!Storage || !InventoryAsset || Quantity <= 0)
	{
		LOG_ERROR(LogTemp, "Storage, Asset is null or Quantity less than or equal to 0");
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = Storage->InventoryRecords;
	FName RecordId = InventoryAsset->AssetId;
	EInventoryItemType AssetType = InventoryAsset->ItemType;

	if (RecordId.IsNone())
	{
		LOG_ERROR(LogTemp, "AssetId is not valid");
		return false;
	}
	
	if (InventoryAsset->bIsStackable)
	{
		FInventoryRecord* Record = Records.Find(RecordId);

		if (Record)
		{
			Record->ItemQuantity += Quantity;
			LOG_INFO(LogTemp, "Stackable record %s updated", *InventoryAsset->AssetName.ToString());
		}
		else {
			FInventoryRecord NewRecord;
			NewRecord.ItemId = RecordId;
			NewRecord.ItemType = AssetType;
			NewRecord.ItemQuantity = Quantity;

			Records.Add(RecordId, NewRecord);
			LOG_INFO(LogTemp, "Stackable record %s added", *InventoryAsset->AssetName.ToString());
		}
	}
	else {
		FInventoryRecord NewRecord;
		NewRecord.ItemId = RecordId;
		NewRecord.ItemType = AssetType;
		NewRecord.ItemQuantity = Quantity;

		Records.Add(FName(FGuid::NewGuid().ToString()), NewRecord);
		LOG_INFO(LogTemp, "Non stackable record added: %s", *InventoryAsset->AssetName.ToString());
	}

	return true;
}

bool UInventorySubsystem::AddRecords(const TMap<UInventoryAsset*, int32>& InventoryAssets, const bool bAllowRollback)
{
	if (!IsValid(Storage))
	{
		LOG_ERROR(LogTemp, "Storage is null");
		return false;
	}

	TMap<FName, FInventoryRecord> RecordsBackup;
	if (bAllowRollback) RecordsBackup = Storage->InventoryRecords;

	for (const auto& Record : InventoryAssets)
	{
		if (!AddRecord(Record.Key, Record.Value) && bAllowRollback)
		{
			Storage->InventoryRecords = RecordsBackup;
			return false;
		}
	}

	return true;
}

bool UInventorySubsystem::RemoveRecord(const FName InventoryRecordId, const int Quantity)
{
	if (!Storage || Quantity <= 0)
	{
		LOG_ERROR(LogTemp, "Storage is null or Quantity less than or equal to 0");
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = Storage->InventoryRecords;
	FInventoryRecord* Record = Records.Find(InventoryRecordId);

	if (!Record)
	{
		LOG_ERROR(LogTemp, "Record not found: %s", *InventoryRecordId.ToString());
		return false;
	}

	if (Record->ItemQuantity > Quantity)
	{
		Record->ItemQuantity -= Quantity;
		LOG_INFO(LogTemp, "Record quantity reduced: %s", *InventoryRecordId.ToString());
	}
	else
	{
		Records.Remove(InventoryRecordId);
		LOG_INFO(LogTemp, "Record removed: %s", *InventoryRecordId.ToString());
	}

	return true;
}

bool UInventorySubsystem::RemoveRecords(const TMap<FName, int32>& InventoryRecordIds, const bool bAllowRollback)
{
	if (!IsValid(Storage))
	{
		LOG_ERROR(LogTemp, "Storage is null");
		return false;
	}

	TMap<FName, FInventoryRecord> RecordsBackup;
	if (bAllowRollback) RecordsBackup = Storage->InventoryRecords;

	for (const auto& Record : InventoryRecordIds)
	{
		if (!RemoveRecord(Record.Key, Record.Value) && bAllowRollback)
		{
			Storage->InventoryRecords = RecordsBackup;
			return false;
		}
	}

	return true;
}

bool UInventorySubsystem::UpdateRecord(const FName InventoryRecordId, FInventoryRecord InventoryRecord)
{
	if (!IsValid(Storage))
	{
		LOG_ERROR(LogTemp, "Storage is null");
		return false;
	}

	TMap<FName, FInventoryRecord>& Records = Storage->InventoryRecords;
	if (!Records.Contains(InventoryRecordId))
	{
		LOG_ERROR(LogTemp, "Record not found: %s", *InventoryRecordId.ToString());
		return false;
	}

	Records.Add(InventoryRecordId, InventoryRecord);
	LOG_INFO(LogTemp, "Record updated: %s", *InventoryRecordId.ToString());
	return true;
}

bool UInventorySubsystem::HasRecord(const FName InventoryRecordId)
{
	if (!IsValid(Storage))
	{
		return false;
	}
	return Storage->InventoryRecords.Contains(InventoryRecordId);
}

FInventoryRecord UInventorySubsystem::GetRecord(const FName InventoryRecordId)
{
	if (!IsValid(Storage))
	{
		return FInventoryRecord();
	}

	FInventoryRecord* Record = Storage->InventoryRecords.Find(InventoryRecordId);
	if (!Record)
	{
		return FInventoryRecord();
	}

	return *Record;
}

TMap<FName, FInventoryRecord> UInventorySubsystem::GetRecords()
{
	return IsValid(Storage) ? Storage->InventoryRecords : TMap<FName, FInventoryRecord>();
}

UInventoryAsset* UInventorySubsystem::GetRecordAsset(const FName InventoryAssetId)
{
	if (!InventoryTable)
	{
		LOG_ERROR(LogTemp, "InventoryTable is null");
		return nullptr;
	}

	FInventoryTable* RecordTable = InventoryTable->FindRow<FInventoryTable>(InventoryAssetId, TEXT(""));
	if (!RecordTable || !RecordTable->InventoryAsset)
	{
		LOG_ERROR(LogTemp, "Record asset not found: %s", *InventoryAssetId.ToString());
		return nullptr;
	}

	return Cast<UInventoryAsset>(RecordTable->InventoryAsset);
}

FInventoryRecord UInventorySubsystem::GetRecordWithAsset(const FName InventoryRecordId, UInventoryAsset*& OutInventoryAsset, bool& bOutFound)
{
	bOutFound = false;

	FInventoryRecord Record = GetRecord(InventoryRecordId);
	if (!Record.IsValid())
	{
		OutInventoryAsset = nullptr;
		return FInventoryRecord();
	}

	OutInventoryAsset = GetRecordAsset(Record.ItemId);
	if (!IsValid(OutInventoryAsset)) {
		OutInventoryAsset = nullptr;
		return FInventoryRecord();
	}

	return Record;
}

void UInventorySubsystem::OverwriteRecords(const TMap<FName, FInventoryRecord>& InventoryRecords)
{
	if (!IsValid(Storage))
	{
		LOG_ERROR(LogTemp, "Storage is null");
		return;
	}

	Storage->InventoryRecords = InventoryRecords;
	LOG_INFO(LogTemp, "Records overwritten");
}

void UInventorySubsystem::PostInitialize_Implementation()
{
	Super::PostInitialize();


	if (UStorageSubsystem* StorageSubsystem = GetGameInstance()->GetSubsystem<UStorageSubsystem>())
	{
		Storage = StorageSubsystem->GetLocalStorage();
		if (!IsValid(Storage))
		{
			LOG_ERROR(LogTemp, "LocalStorage not found");
			return;
		}

		const UGameMetadataSettings* GameMetadata = GetDefault<UGameMetadataSettings>();
		if (!IsValid(GameMetadata) || GameMetadata->InventoryTable.IsNull())
		{
			LOG_ERROR(LogTemp, "GameMetadata or InventoryTable is null");
			return;
		}

		InventoryTable = Cast<UDataTable>(GameMetadata->InventoryTable.LoadSynchronous());
		if (!IsValid(InventoryTable))
		{
			LOG_ERROR(LogTemp, "InventoryTable cast failed");
		}

		LOG_INFO(LogTemp, "Inventory Table & Local Storage loaded");
	}
}

