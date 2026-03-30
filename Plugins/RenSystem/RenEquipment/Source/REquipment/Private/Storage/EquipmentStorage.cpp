// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Storage/EquipmentStorage.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/EquipmentSettings.h"


void UEquipmentStorage::InitializeDefaults()
{
	EquipmentInstances = UEquipmentSettings::Get()->DefaultEquipment;
}

const TMap<FGameplayTag, FEquipmentKey>* UEquipmentStorage::GetOwnedEquipment(const FGuid& OwnerId) const
{
	const FEquipmentInstance* Instance = EquipmentInstances.Find(OwnerId);
	if (!Instance)
	{
		return nullptr;
	}
	return &Instance->EquipmentSlot;
}

bool UEquipmentStorage::GetEquipmentAtSlot(const FGuid& InOwnerId, const FGameplayTag& InEquipmentSlot, FPrimaryAssetId& OutEquipmentAssetId) const
{
	OutEquipmentAssetId = FPrimaryAssetId();

	const FEquipmentInstance* Instance = EquipmentInstances.Find(InOwnerId);
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

bool UEquipmentStorage::SetEquipmentAtSlot(const FGuid& OwnerId, const FPrimaryAssetId& OwnerAssetId, const FGameplayTag& EquipmentSlot, const FGuid& EquipmentId, const FPrimaryAssetId& EquipmentAssetId)
{
	if (!OwnerId.IsValid() || !OwnerAssetId.IsValid() || !EquipmentId.IsValid())
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

	const FGuid* Found_OwnerId = EquipmentRelation.Find(EquipmentId);
	if (Found_OwnerId && *Found_OwnerId != OwnerId)
	{
		LOG_ERROR(LogEquipment, TEXT("Equipment is already owned by another owner"));
		return false;
	}

	FEquipmentInstance& EquipmentInstance = EquipmentInstances.FindOrAdd(OwnerId);
	FPrimaryAssetId& InstanceAssetId = EquipmentInstance.OwnerAssetId;
	if (!InstanceAssetId.IsValid())
	{
		InstanceAssetId = OwnerAssetId;
	}

	FEquipmentKey ExistingKey(EquipmentAssetId, EquipmentId);
	TMap<FGameplayTag, FEquipmentKey>& Slots = EquipmentInstance.EquipmentSlot;

	FGameplayTag ToRemove;
	for (const TPair<FGameplayTag, FEquipmentKey>& Slot : Slots)
	{
		if (Slot.Value == ExistingKey)
		{
			ToRemove = Slot.Key;
			break;
		}
	}

	if (ToRemove.IsValid())
	{
		Slots.Remove(ToRemove);
	}

	Slots.Add(EquipmentSlot, FEquipmentKey(EquipmentAssetId, EquipmentId));
	EquipmentRelation.Add(EquipmentId, OwnerId);

	OnStorageUpdated.Broadcast();
	return true;
}

bool UEquipmentStorage::RemoveEquipmentFromSlot(const FGuid& OwnerId, const FGameplayTag& SlotTag)
{
	FEquipmentInstance* EquipmentInstance = EquipmentInstances.Find(OwnerId);
	if (!EquipmentInstance)
	{
		return false;
	}

	FEquipmentKey Key;
	if (!EquipmentInstance->EquipmentSlot.RemoveAndCopyValue(SlotTag, Key))
	{
		return false;
	}

	EquipmentRelation.Remove(Key.AssetInstanceId);
	OnStorageUpdated.Broadcast();

	return true;
}
