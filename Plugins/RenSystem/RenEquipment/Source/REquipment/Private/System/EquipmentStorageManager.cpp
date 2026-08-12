// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/EquipmentStorageManager.h"

// Project Headers
#include "Core/EquipmentSettings.h"
#include "Data/EquipmentStorage.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


const TMap<FGameplayTag, FEquipmentKey>* UEquipmentStorageManager::GetOwnedEquipment(const FGuid& OwnerId) const
{
	if (!IsValid(LocalStorage))
	{
		return nullptr;
	}

	const FEquipmentInstance* Instance = LocalStorage->EquipmentInstances.Find(OwnerId);
	if (!Instance)
	{
		return nullptr;
	}
	return &Instance->EquipmentSlot;
}

void UEquipmentStorageManager::GetOwnedEquipmentIds(const FGuid& OwnerId, TArray<FGuid>& OutEquipmentIds) const
{
	OutEquipmentIds.Empty();

	if (IsValid(LocalStorage))
	{
		return;
	}

	const FEquipmentInstance* Instance = LocalStorage->EquipmentInstances.Find(OwnerId);
	if (!Instance)
	{
		return;
	}

	for (const TPair<FGameplayTag, FEquipmentKey>& Kv : Instance->EquipmentSlot)
	{
		OutEquipmentIds.Add(Kv.Value.AssetInstanceId);
	}
}

void UEquipmentStorageManager::GetNonOwnedEquipmentIds(const FGuid& OwnerId, TArray<FGuid>& OutEquipmentIds) const
{
	OutEquipmentIds.Empty();

	if (!IsValid(LocalStorage))
	{
		return;
	}

	const TMap<FGuid, FEquipmentInstance>& Instances = LocalStorage->EquipmentInstances;
	for (const TPair<FGuid, FEquipmentInstance>& KvInstance : Instances)
	{
		if (KvInstance.Key != OwnerId)
		{
			for (const TPair<FGameplayTag, FEquipmentKey>& KvSlot : KvInstance.Value.EquipmentSlot)
			{
				OutEquipmentIds.Add(KvSlot.Value.AssetInstanceId);
			}
		}
	}
}


bool UEquipmentStorageManager::GetEquipmentAtSlot(const FGuid& InOwnerId, const FGameplayTag& InEquipmentSlot, FPrimaryAssetId& OutEquipmentAssetId) const
{
	OutEquipmentAssetId = FPrimaryAssetId();

	if (!IsValid(LocalStorage))
	{
		return false;
	}

	const FEquipmentInstance* Instance = LocalStorage->EquipmentInstances.Find(InOwnerId);
	if (!Instance)
	{
		return false;
	}
	
	const TMap<FGameplayTag, FEquipmentKey>& Slots = Instance->EquipmentSlot;
	const FEquipmentKey* FoundId = Slots.Find(InEquipmentSlot);
	if (!FoundId)
	{
		return false;
	}

	OutEquipmentAssetId = FoundId->AssetId;
	return true;
}

bool UEquipmentStorageManager::SetEquipmentAtSlot(const FGuid& OwnerId, const FPrimaryAssetId& OwnerAssetId, const FGameplayTag& EquipmentSlot, const FGuid& EquipmentId, const FPrimaryAssetId& EquipmentAssetId)
{
	if (!IsValid(LocalStorage) || !OwnerId.IsValid() || !OwnerAssetId.IsValid() || !EquipmentId.IsValid())
	{
		LOG_ERROR(LogEquipment, TEXT("Invalid parameters or asset manager"));
		return false;
	}

	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	if (!Settings->EquipmentSlots.HasTagExact(EquipmentSlot))
	{
		LOG_ERROR(LogEquipment, TEXT("Invalid equipment slot"));
		return false;
	}

	if (EquipmentAssetId.PrimaryAssetType != Settings->EquipmentType || OwnerAssetId.PrimaryAssetType != Settings->OwnerType)
	{
		LOG_ERROR(LogEquipment, TEXT("Invalid equipment or owner type"));
		return false;
	}

	TMap<FGuid, FGuid>& Relations = LocalStorage->EquipmentRelations;
	const FGuid* FoundOwnerId = Relations.Find(EquipmentId);
	if (FoundOwnerId && *FoundOwnerId != OwnerId)
	{
		LOG_ERROR(LogEquipment, TEXT("Equipment is already owned by another owner"));
		return false;
	}

	FEquipmentInstance& EquipmentInstance = LocalStorage->EquipmentInstances.FindOrAdd(OwnerId);
	if (!EquipmentInstance.OwnerAssetId.IsValid())
	{
		EquipmentInstance.OwnerAssetId = OwnerAssetId;
	}

	FEquipmentKey ExistingKey(EquipmentAssetId, EquipmentId);
	TMap<FGameplayTag, FEquipmentKey>& Slots = EquipmentInstance.EquipmentSlot;
	
	FGameplayTag SlotToRemove;
	for (const TPair<FGameplayTag, FEquipmentKey>& Slot : Slots)
	{
		if (Slot.Value == ExistingKey)
		{
			SlotToRemove = Slot.Key;
			break;
		}
	}

	if (SlotToRemove.IsValid())
	{
		Slots.Remove(SlotToRemove);
	}

	FEquipmentKey* PreviousKey = Slots.Find(EquipmentSlot);
	if (PreviousKey)
	{
		Relations.Remove(PreviousKey->AssetInstanceId);
	}

	Slots.Add(EquipmentSlot, FEquipmentKey(EquipmentAssetId, EquipmentId));
	Relations.Add(EquipmentId, OwnerId);

	OnStorageUpdated.Broadcast();
	return true;
}

bool UEquipmentStorageManager::RemoveEquipmentFromSlot(const FGuid& OwnerId, const FGameplayTag& SlotTag)
{
	if (!IsValid(LocalStorage))
	{
		return false;
	}

	FEquipmentInstance* EquipmentInstance = LocalStorage->EquipmentInstances.Find(OwnerId);
	if (!EquipmentInstance)
	{
		return false;
	}

	FEquipmentKey Key;
	if (!EquipmentInstance->EquipmentSlot.RemoveAndCopyValue(SlotTag, Key))
	{
		return false;
	}

	LocalStorage->EquipmentRelations.Remove(Key.AssetInstanceId);

	OnStorageUpdated.Broadcast();
	return true;
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
		return LocalStorage->EquipmentRelations.Contains(AssetInstanceId);
	}
	else if (AssetType == Settings->OwnerType)
	{
		return LocalStorage->EquipmentInstances.Contains(AssetInstanceId);
	}

	return false;
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
		LocalStorage->EquipmentInstances = Settings->DefaultEquipment;
		LocalStorage->EquipmentRelations = Settings->DefaultEquipmentRelations;
	}
}

FGameEventDelegate& UEquipmentStorageManager::GetOnStorageUpdated()
{
	return OnStorageUpdated;
}

