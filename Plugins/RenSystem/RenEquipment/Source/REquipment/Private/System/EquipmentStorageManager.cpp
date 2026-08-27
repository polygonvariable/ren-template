// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/EquipmentStorageManager.h"

// Project Headers
#include "Core/EquipmentSettings.h"
#include "Core/Type/EquipmentSpawnData.h"
#include "Data/EquipmentStorage.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


void UEquipmentStorageManager::GetEquipmentByOwnerId(const FGuid& InOwnerInstanceId, TArray<FEquipmentInitializationData>& OutData) const
{
	OutData.Empty();

	if (!IsValid(LocalStorage))
	{
		return;
	}

	const TMap<FGuid, FEquipmentSlotInstance> EquipmentInstances = LocalStorage->EquipmentInstances;
	const FEquipmentOwnerInstance* OwnerInstance = LocalStorage->EquipmentOwners.Find(InOwnerInstanceId);
	if (!OwnerInstance)
	{
		return;
	}

	const TMap<FEquipmentSlotId, FGuid>& Slots = OwnerInstance->Slots;
	for (const TPair<FEquipmentSlotId, FGuid>& Kv : Slots)
	{
		const FEquipmentSlotInstance* SlotInstance = EquipmentInstances.Find(Kv.Value);
		if (!SlotInstance)
		{
			continue;
		}

		FEquipmentInitializationData Data;
		Data.AssetId = SlotInstance->EquipmentAssetId;
		Data.AssetInstanceId = Kv.Value;
		Data.SlotId = Kv.Key;
		
		OutData.Add(FEquipmentInitializationData(SlotInstance->EquipmentAssetId, Kv.Value, Kv.Key));
	}
}

void UEquipmentStorageManager::GetEquipmentIdsByOwnerId(const FGuid& InOwnerInstanceId, bool bInNegate, TArray<FGuid>& OutEquipmentInstanceIds) const
{
	OutEquipmentInstanceIds.Empty();

	if (!IsValid(LocalStorage))
	{
		return;
	}

	const TMap<FGuid, FEquipmentOwnerInstance>& Owners = LocalStorage->EquipmentOwners;

	if (!bInNegate)
	{
		const FEquipmentOwnerInstance* OwnerInstance = Owners.Find(InOwnerInstanceId);
		if (!OwnerInstance)
		{
			return;
		}

		const TMap<FEquipmentSlotId, FGuid>& Slots = OwnerInstance->Slots;
		for (const TPair<FEquipmentSlotId, FGuid>& Kv : Slots)
		{
			OutEquipmentInstanceIds.Add(Kv.Value);
		}
	}
	else
	{
		for (const TPair<FGuid, FEquipmentOwnerInstance>& OwnerKv : Owners)
		{
			if (OwnerKv.Key != InOwnerInstanceId)
			{
				const TMap<FEquipmentSlotId, FGuid>& Slots = OwnerKv.Value.Slots;
				for (const TPair<FEquipmentSlotId, FGuid>& SlotKv : Slots)
				{
					OutEquipmentInstanceIds.Add(SlotKv.Value);
				}
			}
		}
	}
}

bool UEquipmentStorageManager::GetEquipmentAtSlot(const FGuid& InOwnerInstanceId, const FEquipmentSlotId& InSlotId, FPrimaryAssetId& OutEquipmentAssetId) const
{
	OutEquipmentAssetId = FPrimaryAssetId();

	if (!IsValid(LocalStorage))
	{
		return false;
	}

	const FEquipmentOwnerInstance* OwnerInstance = LocalStorage->EquipmentOwners.Find(InOwnerInstanceId);
	if (!OwnerInstance)
	{
		return false;
	}

	const FGuid* EquipmentInstanceId = OwnerInstance->Slots.Find(InSlotId);
	if (!EquipmentInstanceId)
	{
		return false;
	}

	const FEquipmentSlotInstance* SlotInstance =LocalStorage->EquipmentInstances.Find(*EquipmentInstanceId);
	if (!SlotInstance)
	{
		return false;
	}

	OutEquipmentAssetId = SlotInstance->EquipmentAssetId;
	return true;
}

bool UEquipmentStorageManager::SetEquipmentAtSlot(const FGuid& OwnerInstanceId, const FPrimaryAssetId& OwnerAssetId, const FGuid& EquipmentInstanceId, const FPrimaryAssetId& EquipmentAssetId, const FEquipmentSlotId& SlotId)
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}

	TMap<FGuid, FEquipmentOwnerInstance>& EquipmentOwners = LocalStorage->EquipmentOwners;
	TMap<FGuid, FEquipmentSlotInstance>& EquipmentInstances = LocalStorage->EquipmentInstances;

	const FEquipmentSlotInstance* FoundSlotInstance = EquipmentInstances.Find(EquipmentInstanceId);
	if (FoundSlotInstance && FoundSlotInstance->OwnerInstanceId != OwnerInstanceId)
	{
		LOG_ERROR(LogEquipment, TEXT("Equipment is already owned by another owner"));
		return false;
	}

	FEquipmentOwnerInstance& OwnerInstance = EquipmentOwners.FindOrAdd(OwnerInstanceId);
	if (!OwnerInstance.OwnerAssetId.IsValid())
	{
		OwnerInstance.OwnerAssetId = OwnerAssetId;
	}

	FEquipmentSlotId SlotToRemove;
	TMap<FEquipmentSlotId, FGuid>& OwnerSlots = OwnerInstance.Slots;
	for (const TPair<FEquipmentSlotId, FGuid>& Kv : OwnerSlots)
	{
		if (Kv.Value == EquipmentInstanceId)
		{
			SlotToRemove = Kv.Key;
			break;
		}
	}

	if (SlotToRemove.IsValid())
	{
		OwnerSlots.Remove(SlotToRemove);
	}

	FGuid* PreviousEquipment = OwnerSlots.Find(SlotId);
	if (PreviousEquipment)
	{
		EquipmentInstances.Remove(*PreviousEquipment);
	}

	OwnerSlots.Add(SlotId, EquipmentInstanceId);
	EquipmentInstances.Add(EquipmentInstanceId, FEquipmentSlotInstance(EquipmentAssetId, OwnerInstanceId));

	OnStorageUpdated.Broadcast();
	return true;

}

bool UEquipmentStorageManager::RemoveEquipmentFromSlot(const FGuid& OwnerInstanceId, const FEquipmentSlotId& SlotId)
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}

	TMap<FGuid, FEquipmentOwnerInstance>& EquipmentOwners = LocalStorage->EquipmentOwners;
	TMap<FGuid, FEquipmentSlotInstance>& EquipmentInstances = LocalStorage->EquipmentInstances;

	const FEquipmentOwnerInstance* OwnerInstance = EquipmentOwners.Find(OwnerInstanceId);
	if (!OwnerInstance)
	{
		return false;
	}

	const FGuid* EquipmentInstanceId = OwnerInstance->Slots.Find(SlotId);
	if (!EquipmentInstanceId)
	{
		return false;
	}

	EquipmentInstances.Remove(*EquipmentInstanceId);
	OnStorageUpdated.Broadcast();
	return true;
}


UObject* UEquipmentStorageManager::GetStorage() const
{
	return LocalStorage;
}

void UEquipmentStorageManager::SetStorage(UObject* Storage)
{
	LocalStorage = Cast<UEquipmentStorage>(Storage);
}

void UEquipmentStorageManager::OnStorageLoaded(bool bIsNew)
{
	if (IsValid(LocalStorage) && bIsNew)
	{
		const UEquipmentSettings* Settings = UEquipmentSettings::Get();
		LocalStorage->EquipmentOwners = Settings->DefaultEquipmentOwners;
		LocalStorage->EquipmentInstances = Settings->DefaultEquipmentInstances;
	}
}

FGameEventDelegate& UEquipmentStorageManager::GetOnStorageUpdated()
{
	return OnStorageUpdated;
}


bool UEquipmentStorageManager::HasLinkedInstance(const FPrimaryAssetId& AssetId, const FGuid& AssetInstanceId) const
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}

	const FPrimaryAssetType& AssetType = AssetId.PrimaryAssetType;
	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	if (AssetType == Settings->EquipmentType)
	{
		return LocalStorage->EquipmentInstances.Contains(AssetInstanceId);
	}
	else if (AssetType == Settings->OwnerType)
	{
		return LocalStorage->EquipmentOwners.Contains(AssetInstanceId);
	}

	return false;
}

