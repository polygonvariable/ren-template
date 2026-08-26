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

	const TMap<FEquipmentSlotDefinition, FGuid>& Slots = OwnerInstance->Slots;
	for (const TPair<FEquipmentSlotDefinition, FGuid>& Kv : Slots)
	{
		const FEquipmentSlotInstance* SlotInstance = EquipmentInstances.Find(Kv.Value);
		if (!SlotInstance)
		{
			continue;
		}

		FEquipmentInitializationData Data;
		Data.AssetId = SlotInstance->EquipmentAssetId;
		Data.AssetInstanceId = Kv.Value;
		Data.SlotDefinition = Kv.Key;
		
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

		const TMap<FEquipmentSlotDefinition, FGuid>& Slots = OwnerInstance->Slots;
		for (const TPair<FEquipmentSlotDefinition, FGuid>& Kv : Slots)
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
				const TMap<FEquipmentSlotDefinition, FGuid>& Slots = OwnerKv.Value.Slots;
				for (const TPair<FEquipmentSlotDefinition, FGuid>& SlotKv : Slots)
				{
					OutEquipmentInstanceIds.Add(SlotKv.Value);
				}
			}
		}
	}
}

bool UEquipmentStorageManager::GetEquipmentAtSlot(const FGuid& InOwnerInstanceId, const FEquipmentSlotDefinition& InSlotDefinition, FPrimaryAssetId& OutEquipmentAssetId) const
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

	const FGuid* EquipmentInstanceId = OwnerInstance->Slots.Find(InSlotDefinition);
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

bool UEquipmentStorageManager::SetEquipmentAtSlot(const FGuid& OwnerInstanceId, const FPrimaryAssetId& OwnerAssetId, const FGuid& EquipmentInstanceId, const FPrimaryAssetId& EquipmentAssetId, const FEquipmentSlotDefinition& SlotDefinition)
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

	FEquipmentSlotDefinition SlotToRemove;
	TMap<FEquipmentSlotDefinition, FGuid>& OwnerSlots = OwnerInstance.Slots;
	for (const TPair<FEquipmentSlotDefinition, FGuid>& Kv : OwnerSlots)
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

	FGuid* PreviousEquipment = OwnerSlots.Find(SlotDefinition);
	if (PreviousEquipment)
	{
		EquipmentInstances.Remove(*PreviousEquipment);
	}

	OwnerSlots.Add(SlotDefinition, EquipmentInstanceId);
	EquipmentInstances.Add(EquipmentInstanceId, FEquipmentSlotInstance(EquipmentAssetId, OwnerInstanceId));

	OnStorageUpdated.Broadcast();
	return true;

}

bool UEquipmentStorageManager::RemoveEquipmentFromSlot(const FGuid& OwnerInstanceId, const FEquipmentSlotDefinition& SlotDefinition)
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

	const FGuid* EquipmentInstanceId = OwnerInstance->Slots.Find(SlotDefinition);
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

